#include "Linker.h"
#include "ModuleSystemCommon.h"
#include "SceModuleSystem.h"
#include "Loader/FuncStub.h"
#include "Platform/PlatformUtils.h"

LOG_CHANNEL(Linker);

CLinker::CLinker() : m_modSystem{*CSceModuleSystem::GetInstance()} {}

// resolveSymbol always returns true
bool CLinker::resolveSymbol(NativeModule const &mod,
							std::string const &name,
							uint64_t *addrOut) const
{
	bool retVal = true;

	do
	{
		const SymbolInfo *info = nullptr;
		void *address    = nullptr;
		bool useNative   = false;

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
			address = getSymbolAddress(info->moduleName,
									   info->libraryName,
									   info->symbolName);

			auto policy = m_modSystem.getSymbolPolicy(info->moduleName,
													  info->libraryName,
													  info->symbolName);

			useNative  = (policy == Policy::UseNative);
		}
		else
		{
			address = getSymbolAddress(info->moduleName,
									   info->libraryName,
									   info->nid);

			auto policy = m_modSystem.getSymbolPolicy(info->moduleName,
													  info->libraryName,
													  info->nid);

			useNative  = (policy == Policy::UseNative);
		}

		const char* source = useNative? "NATIVE":"BUILTIN";

		LOG_ERR_IF(address == nullptr, "fail to resolve symbol: %s[%s] from %s for module %s",
				   name.c_str(), source, info->moduleName.c_str(), mod.fileName.c_str());

#ifdef MODSYS_STUB_DISABLE

		if (!address)
		{
			break;
		}
		*addrOut = reinterpret_cast<uint64_t>(address);

#else // MODSYS_STUB_DISABLE

		if (address != nullptr && !useNative)
		{
			// builtin function
			*addrOut = reinterpret_cast<uint64_t>(address);
		}
		else if (address != nullptr && useNative)
		{
			// native function

#ifdef MODSYS_STUB_ON_NATIVE
			*addrOut = reinterpret_cast<uint64_t>(generateStubFunction(info, address));
#else  // MODSYS_STUB_ON_NATIVE
			*addrOut = reinterpret_cast<uint64_t>(address);
#endif  // MODSYS_STUB_ON_NATIVE

		}
		else
		{
			// unknown function

#ifdef MODSYS_STUB_ON_UNKNOWN
			*addrOut = reinterpret_cast<uint64_t>(generateStubFunction(info, address));
#else   // MODSYS_STUB_ON_UNKNOWN
			*addrOut = reinterpret_cast<uint64_t>(address);
#endif  // MODSYS_STUB_ON_UNKNOWN

		}


#endif  // MODSYS_STUB_DISABLE

		retVal = true;
	} while (false);

	return retVal;
}


void* CLinker::generateStubFunction(const SymbolInfo* sybInfo, void* oldFunc) const
{
	void* stubFunc = nullptr;
	do 
	{
		const char* formatString = nullptr;

		if (oldFunc == nullptr)
		{
			// NOTE: Something is wrong with va_args and u64 values, so print NID as 2 u32
			formatString =
				"Unknown Function nid 0x%08X%08X (\"%s\") from lib:%s is called";
		}
		else
		{
			formatString =
				"Function nid 0x%08x%08x (\"%s\") from lib:%s is called at %p";
		}

		auto nidString = sybInfo->symbolName.substr(0, 11);

		auto msg = UtilString::Format(formatString,
									  sybInfo->nid >> 32,
									  sybInfo->nid,
									  nidString.c_str(),
									  sybInfo->libraryName.c_str(),
									  oldFunc);

		auto stubMgr  = GetFuncStubManager();
		stubFunc      = oldFunc == nullptr ? 
			stubMgr->generateUnknown(msg) : 
			stubMgr->generate(msg, oldFunc);

	} while (false);
	return stubFunc;
}


bool CLinker::relocateModules()
{
	auto &mods  = m_modSystem.getAllNativeModules();
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

void* CLinker::getSymbolAddress(std::string const& modName,
								std::string const& libName,
							    uint64_t nid) const
{
	auto pointer = m_modSystem.getSymbolAddress(modName, libName, nid);
	return const_cast<void*>(pointer);
}

void* CLinker::getSymbolAddress(std::string const& modName, std::string const& libName, std::string const& symbName) const
{
	auto symbolManager = m_modSystem.getSymbolManager();
	auto policy = m_modSystem.getSymbolPolicy(modName, libName, symbName);
	const void* pointer = nullptr;

	if (policy == Policy::UseBuiltin)
	{
		pointer = symbolManager.findBuiltinSymbol(modName, libName, symbName);
	}
	else // Policy::UseNative
	{
		pointer = symbolManager.findNativeSymbol(modName, libName, symbName);
	}

	return const_cast<void*>(pointer);
}

bool CLinker::relocateModule(NativeModule& mod)
{
	bool retVal = false;

	if (relocateRela(mod) && relocatePltRela(mod))
	{
		retVal = true;
	}

	return retVal;
}

bool CLinker::relocateRela(NativeModule &mod)
{
	bool retVal = false;
	do
	{
		if (mod.getFileMemory().empty())
		{
			break;
		}

		auto &info = mod.getModuleInfo();

		uint8_t *pImageBase         = info.pCodeAddr;
		uint8_t *pStrTab            = info.pStrTab;
		Elf64_Sym *pSymTab       = (Elf64_Sym *)info.pSymTab;
		Elf64_Rela *pRelaEntries = (Elf64_Rela *)info.pRela;
		for (uint32_t i = 0; i != info.nRelaCount; ++i)
		{
			Elf64_Rela *pRela = &pRelaEntries[i];
			auto nType        = ELF64_R_TYPE(pRela->r_info);
			auto nSymIdx      = ELF64_R_SYM(pRela->r_info);

			switch (nType)
			{
			case R_X86_64_NONE:
			case R_X86_64_PC32:
			case R_X86_64_COPY:
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
				uint64_t nSymVal    = 0;

				if (nBinding == STB_LOCAL)
				{
					nSymVal = (uint64_t)(pImageBase + symbol.st_value);
				}
				else if (nBinding == STB_GLOBAL || nBinding == STB_WEAK)
				{
					auto pName = (const char *)&pStrTab[symbol.st_name];
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

				*(uint64_t *)&pImageBase[pRela->r_offset] =
					nSymVal + pRela->r_addend;
			}
			break;
			case R_X86_64_GLOB_DAT:
			{
				Elf64_Sym& symbol = pSymTab[nSymIdx];
				auto nBinding     = ELF64_ST_BIND(symbol.st_info);
				uint64_t nSymVal    = 0;
				auto pName       = (const char*)&pStrTab[symbol.st_name];
				//LOG_DEBUG("RELA symbol: %s", pName);
				if (!resolveSymbol(mod, pName, &nSymVal))
				{
					*(uint64_t*)&pImageBase[pRela->r_offset] = 0;
					LOG_ERR("can not get symbol address.");
					break;
				}
				*(uint64_t*)&pImageBase[pRela->r_offset] =
					nSymVal + pRela->r_addend;
			}
			break;
			case R_X86_64_RELATIVE:
			{
				*(uint64_t *)&pImageBase[pRela->r_offset] =
					(uint64_t)(pImageBase + pRela->r_addend);
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

bool CLinker::relocatePltRela(NativeModule &mod)
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

		uint8_t *pImageBase         = info.pCodeAddr;
		uint8_t *pStrTab            = info.pStrTab;
		Elf64_Sym *pSymTab       = (Elf64_Sym *)info.pSymTab;
		Elf64_Rela *pRelaEntries = (Elf64_Rela *)info.pPltRela;

		LOG_DEBUG("PLT RELA count: %d, for library: %s", info.nPltRelaCount, mod.fileName.c_str());
		for (uint32_t i = 0; i != info.nPltRelaCount; ++i)
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
				uint64_t nSymVal    = 0;

				if (nBinding == STB_LOCAL)
				{
					nSymVal = (uint64_t)(pImageBase + symbol.st_value);
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

				*(uint64_t *)&pImageBase[pRela->r_offset] = nSymVal;
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
