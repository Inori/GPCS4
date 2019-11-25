#include "Linker.h"
#include "ModuleSystemCommon.h"
#include "SceModuleSystem.h"
#include "Loader/FuncStub.h"
#include "Platform/PlatformUtils.h"

CLinker::CLinker() : m_modSystem{*CSceModuleSystem::GetInstance()} {}

bool CLinker::ResolveSymbol(const std::string &strModName,
							const std::string &strLibName,
							uint64 nNid,
							void **ppAddress) const
{
	bool bRet = false;
	do
	{
		if (!ppAddress)
		{
			break;
		}

		void *pFunc = CSceModuleSystem::GetInstance()->FindFunction(
			strModName, strLibName, nNid);
		if (!pFunc)
		{
			break;
		}

		*ppAddress = pFunc;

		bRet = true;
	} while (false);
	return bRet;
}

// resolveSymbol always returns true
bool CLinker::resolveSymbol(MemoryMappedModule const &mod,
							std::string const &name,
							uint64_t *addrOut) const
{
	bool retVal = true;

	do
	{
		const SymbolInfo *info = nullptr;
		void *address          = nullptr;
		bool overridden         = false;

		if (addrOut == nullptr)
		{
			LOG_ERR("null pointer");
			break;
		}

		auto ret = mod.getSymbol(name, &info);
		if (!ret)
		{
			LOG_ERR("fail to find symbol: %s", name.c_str());
			break;
		}

		if (!info->isEncoded)
		{
			address = m_modSystem.findSymbol(info->moduleName, info->libraryName,
								   info->moduleName);
		}
		else
		{
			address = m_modSystem.FindFunction(info->moduleName, info->libraryName,
											   info->nid);

			overridden = m_modSystem.isFunctionOverridable(info->moduleName,
														  info->libraryName,
														  info->nid);
		}

		if (address == nullptr)
		{
			LOG_ERR("fail to resolve symbol: %s from %s for module %s",
					name.c_str(), info->moduleName.c_str(), mod.fileName.c_str());
		}

		if (!overridden && address != nullptr || !USE_FUNCTION_STUBS)
		{
			*addrOut = reinterpret_cast<uint64_t>(address);
		}
		else if (LOG_UNKNOWN_FUNCTION_ONLY && address != nullptr)
		{
			*addrOut  = reinterpret_cast<uint64_t>(address);
		}
		else // Use function stub 
		{
			const char *formatString = nullptr;

			if (address == nullptr)
			{
				formatString =
					"Unknown Function nid 0x%016x from lib:%s is called: 0x%08x";
			}
			else
			{
				formatString =
					"Function nid 0x%016x from lib:%s is called";
			}

			auto msg = UtilString::Format(formatString,
										  info->nid,
										  info->libraryName.c_str());

			auto stubMgr  = GetFuncStubManager();
			auto stubAddr = address == nullptr? 
								stubMgr->generateUnknown(msg):
								stubMgr->generate(msg, address);

			*addrOut = reinterpret_cast<uint64_t>(stubAddr);
		}

		retVal = true;
	} while (false);

	return retVal;
}

bool CLinker::relocateModules()
{
	auto &mods  = m_modSystem.getMemoryMappedModules();
	bool retVal = false;

	for (auto &mod : mods)
	{
		retVal = relocateModule(mod);
		if (retVal == false)
		{
			LOG_ERR("fail to relocate module: %s", mod.fileName.c_str());
			break;
		}
	}

	return retVal;
}

bool CLinker::relocateModule(MemoryMappedModule &mod)
{
	bool retVal = false;

	if (relocateRela(mod) && relocatePltRela(mod))
	{
		retVal = true;
	}

	return retVal;
}

bool CLinker::relocateRela(MemoryMappedModule &mod)
{
	bool retVal = false;
	do
	{
		if (mod.getFileMemory().empty())
		{
			break;
		}

		auto &info = mod.getModuleInfo();

		byte *pImageBase         = info.pCodeAddr;
		byte *pStrTab            = info.pStrTab;
		Elf64_Sym *pSymTab       = (Elf64_Sym *)info.pSymTab;
		Elf64_Rela *pRelaEntries = (Elf64_Rela *)info.pRela;
		for (uint i = 0; i != info.nRelaCount; ++i)
		{
			Elf64_Rela *pRela = &pRelaEntries[i];
			auto nType        = ELF64_R_TYPE(pRela->r_info);
			auto nSymIdx      = ELF64_R_SYM(pRela->r_info);

			switch (nType)
			{
			case R_X86_64_NONE:
			case R_X86_64_PC32:
			case R_X86_64_COPY:
			case R_X86_64_GLOB_DAT:
			case R_X86_64_TPOFF64:
			case R_X86_64_TPOFF32:
			case R_X86_64_DTPMOD64:
			case R_X86_64_DTPOFF64:
			case R_X86_64_DTPOFF32:
				break;
			case R_X86_64_64:
			{
				Elf64_Sym &symbol = pSymTab[nSymIdx];
				auto nBinding     = ELF64_ST_BIND(symbol.st_info);
				uint64 nSymVal    = 0;

				if (nBinding == STB_LOCAL)
				{
					nSymVal = (uint64)(pImageBase + symbol.st_value);
				}
				else if (nBinding == STB_GLOBAL || nBinding == STB_WEAK)
				{
					char *pName = (char *)&pStrTab[symbol.st_name];
					//LOG_DEBUG("RELA symbol: %s", pName);
					if (!resolveSymbol(mod, pName, &nSymVal))
					{
						LOG_ERR("can not get symbol address.");
						break;
					}
				}
				else
				{
					LOG_ERR("invalid sym bingding %d", nBinding);
				}

				*(uint64 *)&pImageBase[pRela->r_offset] =
					nSymVal + pRela->r_addend;
			}
			break;
			case R_X86_64_RELATIVE:
			{
				*(uint64 *)&pImageBase[pRela->r_offset] =
					(uint64)(pImageBase + pRela->r_addend);
			}
			break;
			default:
				LOG_FIXME("rela type not handled %d", nType);
				break;
			}
		}
		retVal = true;

	} while (false);

	return retVal;
}

bool CLinker::relocatePltRela(MemoryMappedModule &mod)
{
	bool bRet = false;
	do
	{
		auto &fileData = mod.getFileMemory();
		auto &info     = mod.getModuleInfo();

		if (fileData.empty())
		{
			break;
		}

		byte *pImageBase         = info.pCodeAddr;
		byte *pStrTab            = info.pStrTab;
		Elf64_Sym *pSymTab       = (Elf64_Sym *)info.pSymTab;
		Elf64_Rela *pRelaEntries = (Elf64_Rela *)info.pPltRela;

		LOG_DEBUG("PLT RELA count: %d", info.nPltRelaCount);
		for (uint i = 0; i != info.nPltRelaCount; ++i)
		{
			Elf64_Rela *pRela = &pRelaEntries[i];
			auto nType        = ELF64_R_TYPE(pRela->r_info);
			auto nSymIdx      = ELF64_R_SYM(pRela->r_info);

			switch (nType)
			{
			case R_X86_64_JUMP_SLOT:
			{
				Elf64_Sym &symbol = pSymTab[nSymIdx];
				auto nBinding     = ELF64_ST_BIND(symbol.st_info);
				uint64 nSymVal    = 0;

				if (nBinding == STB_LOCAL)
				{
					nSymVal = (uint64)(pImageBase + symbol.st_value);
				}
				else if (nBinding == STB_GLOBAL || nBinding == STB_WEAK)
				{
					char *pName = (char *)&pStrTab[symbol.st_name];
					//LOG_DEBUG("PLT RELA symbol: %s", pName);
					if (!resolveSymbol(mod, pName, &nSymVal))
					{
						LOG_ERR("can not get symbol address.");
						//break;
					}
				}
				else
				{
					LOG_ERR("invalid sym bingding %d", nBinding);
				}

				*(uint64 *)&pImageBase[pRela->r_offset] = nSymVal;
			}
			break;
			default:
			{
				LOG_FIXME("index:%d:rela type not handled %d",i, nType);
			}
			break;
			}
		}
		bRet = true;
	} while (false);

	return bRet;
}
