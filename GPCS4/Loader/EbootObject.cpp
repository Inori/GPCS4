#if 0 // Legacy code
#include "EbootObject.h"
#include "Platform/PlatformUtils.h"
#include <algorithm>

#define SCE_HEADER_LEN 0x160


CEbootObject::CEbootObject()
{
	memset(&m_stModuleInfo, 0, sizeof(MODULE_INFO));
}

CEbootObject::~CEbootObject()
{
}

bool CEbootObject::Load(const std::string& strFileName)
{
	bool bRet = false;
	FILE* fin = nullptr;
	do
	{
		fin = fopen(strFileName.c_str(), "rb");
		if (!fin)
		{
			LOG_ERR("can not open eboot file: %s", strFileName.c_str());
			break;
		}

		uint nFileSize = 0;
		fseek(fin, 0, SEEK_END);
		nFileSize = ftell(fin);
		fseek(fin, 0, SEEK_SET);

		m_vtFileMem.resize(nFileSize);

		fread(m_vtFileMem.data(), nFileSize, 1, fin);

		if (!MapElf())
		{
			break;
		}

		bRet = true;
	} while (false);
	if (fin)
	{
		fclose(fin);
	}
	return bRet;
}

void CEbootObject::Unload()
{
	m_vtFileMem.clear();

	if (m_stModuleInfo.pMappedAddr)
	{
		UtilMemory::VMUnMap(m_stModuleInfo.pMappedAddr, m_stModuleInfo.nMappedSize);
	}

	memset(&m_stModuleInfo, 0, sizeof(MODULE_INFO));
}

void* CEbootObject::EntryPoint()
{
	return m_stModuleInfo.pEntryPoint;
}

bool CEbootObject::GetTlsInfo(void** pTls, uint& nInitSize, uint& nTotalSize)
{
	bool bRet = false;
	do
	{
		if (!pTls)
		{
			break;
		}

		*pTls = m_stModuleInfo.pTlsAddr;
		nInitSize = m_stModuleInfo.nTlsInitSize;
		nTotalSize = m_stModuleInfo.nTlsSize;

		bRet  = true;
	}while(false);
	return bRet;
}

void CEbootObject::SetLinker(std::shared_ptr<CLinker>& pLinker)
{
	m_pLinker = pLinker;
}

bool CEbootObject::MapElf()
{
	bool bRet = false;
	do
	{
		memcpy(&m_stEhdr, m_vtFileMem.data(), sizeof(m_stEhdr));
		if (strncmp((char*)m_stEhdr.e_ident, ELFMAG, SELFMAG))
		{
			break;
		}

		if (m_stEhdr.e_type != ET_SCE_DYNEXEC || m_stEhdr.e_machine != EM_X86_64)
		{
			break;
		}

		byte* pPhStart = m_vtFileMem.data() + m_stEhdr.e_phoff;
		uint nPhCount = m_stEhdr.e_phnum;
		m_vtPhdrs.resize(nPhCount);
		memcpy(&m_vtPhdrs[0], pPhStart, nPhCount * sizeof(Elf64_Phdr));

		if (!CollectModuleInfo())
		{
			LOG_ERR("collect module info failed.");
			break;
		}

		if (!LoadInVitrualMemory())
		{
			break;
		}

		if (!ProcessSegments())
		{
			LOG_ERR("parse segments failed.");
			break;
		}

		//do not need anymore, clear it to release memory
		//note: some fields in m_stModuleInfo will be invalid after this operation
		m_vtFileMem.clear();

		bRet = true;
	} while (false);
	return bRet;
}

bool CEbootObject::CollectModuleInfo()
{
	bool bRet = false;
	do 
	{
		if (m_vtFileMem.empty())
		{
			break;
		}

		byte* pBuffer = m_vtFileMem.data();

		for (Elf64_Phdr& phdr : m_vtPhdrs)
		{
			if (phdr.p_type == PT_LOAD)
			{
				continue;
			}

			switch (phdr.p_type)
			{
			case PT_SCE_RELRO:
				break;
			case PT_SCE_PROCPARAM:
				break;
			case PT_DYNAMIC:
			{
				m_stModuleInfo.pDynamic = pBuffer + phdr.p_offset;
				m_stModuleInfo.nDynamicSize = phdr.p_filesz;
			}
				break;
			case PT_INTERP:
				break;
			case PT_TLS:
			{
				m_stModuleInfo.pTlsAddr = (byte*)phdr.p_vaddr;  // neet to rebase when load
				m_stModuleInfo.nTlsInitSize = phdr.p_filesz;
				m_stModuleInfo.nTlsSize = ALIGN_ROUND(phdr.p_memsz, phdr.p_align);
			}
				break;
			case PT_GNU_EH_FRAME:
				break;
			case PT_SCE_DYNLIBDATA:
			{
				m_stModuleInfo.pSceDynLib = pBuffer + phdr.p_offset;
				m_stModuleInfo.nSceDynLibSize = phdr.p_filesz;
			}
				break;
			case PT_SCE_COMMENT:
			{
				m_stModuleInfo.pSceComment = pBuffer + phdr.p_offset;
				m_stModuleInfo.nSceCommentSize = phdr.p_filesz;
			}
				break;
			case PT_SCE_LIBVERSION:
			{
				m_stModuleInfo.pSceLibVersion = pBuffer + phdr.p_offset;
				m_stModuleInfo.nSceLibVersionSize = phdr.p_filesz;
			}
				break;
			default:
			{
				LOG_FIXME("do not support segment type %08X", phdr.p_type);
				bRet = false;
			}
				break;
			}
		}

		bRet = ParseDynamic();

		if (bRet)
		{
			break;
		}

		bRet = true;
	} while (false);
	return bRet;
}

bool CEbootObject::PreParseDynamic()
{
	bool bRet = false;
	do 
	{
		Elf64_Dyn* pDynEntries = (Elf64_Dyn*)m_stModuleInfo.pDynamic;
		uint nDynEntryCount = m_stModuleInfo.nDynamicSize / sizeof(Elf64_Dyn);

		for (uint i = 0; i != nDynEntryCount; ++i)
		{
			Elf64_Dyn* pDyn = &pDynEntries[i];
			switch (pDyn->d_tag)
			{
			case DT_NULL:
				break;
			case DT_NEEDED:
				break;
			case DT_DEBUG:
			case DT_TEXTREL:
			case DT_INIT_ARRAY:
			case DT_FINI_ARRAY:
			case DT_INIT_ARRAYSZ:
			case DT_FINI_ARRAYSZ:
			case DT_FLAGS:
			case DT_PREINIT_ARRAY:
			case DT_PREINIT_ARRAYSZ:
				break;

			case DT_SCE_FINGERPRINT:
			case DT_SCE_ORIGINAL_FILENAME:
			case DT_SCE_MODULE_INFO:
				break;
			case DT_SCE_NEEDED_MODULE:
				break;
			case DT_SCE_MODULE_ATTR:
				break;
			case DT_SCE_EXPORT_LIB:
				break;
			case DT_SCE_IMPORT_LIB:
				break;
			case DT_SCE_EXPORT_LIB_ATTR:
			case DT_SCE_IMPORT_LIB_ATTR:
			case DT_SCE_RELAENT:
			case DT_SCE_SYMENT:
				break;

			case DT_SCE_HASH:
			case DT_SCE_HASHSZ:
				break;

				/* general info */
			case DT_INIT:
				LOG_DEBUG("INIT addr: %08x", pDyn->d_un.d_ptr);
				break;
			case DT_FINI:
				LOG_DEBUG("FINI addr: %08x", pDyn->d_un.d_ptr);
				break;
			case DT_SCE_PLTGOT:
				LOG_DEBUG("PLTGOT addr: %08x", pDyn->d_un.d_ptr);
				break;

				/* symbol table info */
			case DT_SCE_SYMTAB:
			{
				LOG_DEBUG("    %i: d_tag = %s, d_un = %08x", i, "DT_SCE_SYMTAB", pDyn->d_un.d_val);
				m_stModuleInfo.pSymTab = m_stModuleInfo.pSceDynLib + pDyn->d_un.d_ptr;
			}
			break;
			case DT_SCE_SYMTABSZ:
			{
				LOG_DEBUG("    %i: d_tag = %s, d_un = %08x", i, "DT_SCE_SYMTABSZ", pDyn->d_un.d_val);
				m_stModuleInfo.nSymTabSize = pDyn->d_un.d_val;
			}
			break;

			/* string table info */
			case DT_SCE_STRTAB:
			{
				LOG_DEBUG("    %i: d_tag = %s, d_un = %08x", i, "DT_SCE_STRTAB", pDyn->d_un.d_val);
				m_stModuleInfo.pStrTab = m_stModuleInfo.pSceDynLib + pDyn->d_un.d_ptr;
			}
			break;
			case DT_SCE_STRSZ:
			{
				LOG_DEBUG("    %i: d_tag = %s, d_un = %08x", i, "DT_SCE_STRSZ", pDyn->d_un.d_val);
				m_stModuleInfo.nStrTabSize = pDyn->d_un.d_val;
			}
			break;

			/* relocation table info */
			case DT_SCE_RELA:
			{
				LOG_DEBUG("    %i: d_tag = %s, d_un = %08x", i, "DT_SCE_RELA", pDyn->d_un.d_val);
				m_stModuleInfo.pRela = m_stModuleInfo.pSceDynLib + pDyn->d_un.d_ptr;
			}
			break;
			case DT_SCE_RELASZ:
			{
				LOG_DEBUG("    %i: d_tag = %s, d_un = %08x", i, "DT_SCE_RELASZ", pDyn->d_un.d_val);
				m_stModuleInfo.nRelaCount = pDyn->d_un.d_val / sizeof(Elf64_Rela);
			}
			break;

			/* jmpslots info */
			case DT_SCE_JMPREL:
			{
				LOG_DEBUG("    %i: d_tag = %s, d_un = %08x", i, "DT_SCE_JMPREL", pDyn->d_un.d_val);
				m_stModuleInfo.pPltRela = m_stModuleInfo.pSceDynLib + pDyn->d_un.d_ptr;
			}
			break;
			case DT_SCE_PLTREL:
				LOG_DEBUG("    %i: d_tag = %s, d_un = %08x", i, "DT_SCE_PLTREL", pDyn->d_un.d_val);
				break;
			case DT_SCE_PLTRELSZ:
			{
				LOG_DEBUG("    %i: d_tag = %s, d_un = %08x", i, "DT_SCE_PLTRELSZ", pDyn->d_un.d_val);
				m_stModuleInfo.nPltRelaCount = pDyn->d_un.d_val / sizeof(Elf64_Rela);
			}
			break;
			default:
				LOG_FIXME("do not support dynamic tag %08X", pDyn->d_tag);
				break;
			}
		}

		bRet = true;
	} while (false);
	return bRet;
}

bool CEbootObject::ParseDynamic()
{
	bool bRet = false;
	do
	{

		if (!PreParseDynamic())
		{
			break;
		}

		Elf64_Dyn* pDynEntries = (Elf64_Dyn*)m_stModuleInfo.pDynamic;
		uint nDynEntryCount = m_stModuleInfo.nDynamicSize / sizeof(Elf64_Dyn);
		byte* pStrTab = m_stModuleInfo.pStrTab;

		for (uint i = 0; i != nDynEntryCount; ++i)
		{
			Elf64_Dyn* pDyn = &pDynEntries[i];
			switch (pDyn->d_tag)
			{
			case DT_NEEDED:
			{
				char* pName = (char*)&pStrTab[pDyn->d_un.d_ptr];
				LOG_DEBUG("DT_NEEDED: %s", pName);
			}
				break;
			case DT_SCE_MODULE_INFO:
			{
				//for ps4 game, this is the game's export module
				//which we don't need
				IMPORT_MODULE stMod;
				stMod.value = pDyn->d_un.d_val;
				stMod.strName = (char*)&pStrTab[stMod.name_offset];
				LOG_DEBUG("DT_SCE_MODULE_INFO: %s", stMod.strName.c_str());
			}
				break;
			case DT_SCE_NEEDED_MODULE:
			{
				IMPORT_MODULE stMod;
				stMod.value = pDyn->d_un.d_val;
				stMod.strName = (char*)&pStrTab[stMod.name_offset];
				m_vtModules.push_back(stMod);
				LOG_DEBUG("DT_SCE_NEEDED_MODULE: %s", stMod.strName.c_str());
			}
				break;
			case DT_SCE_MODULE_ATTR:
				break;
			case DT_SCE_EXPORT_LIB:
			{
				//for ps4 game, this is the game's export library
				//which we don't need
				IMPORT_LIBRARY stLib;
				stLib.value = pDyn->d_un.d_val;
				stLib.strName = (char*)&pStrTab[stLib.name_offset];
				LOG_DEBUG("DT_SCE_EXPORT_LIB %s", stLib.strName.c_str());
			}
				break;
			case DT_SCE_IMPORT_LIB:
			{
				IMPORT_LIBRARY stLib;
				stLib.value = pDyn->d_un.d_val;
				stLib.strName = (char*)&pStrTab[stLib.name_offset];
				m_vtLibraries.push_back(stLib);
				LOG_DEBUG("DT_SCE_IMPORT_LIB %s", stLib.strName.c_str());
			}
				break;
			default:
				break;
			}
		}

		bRet = true;
	} while (false);
	return bRet;
}

bool CEbootObject::ProcessSegments()
{
	bool bRet = false;
	do 
	{
		if (!Relocate())
		{
			break;
		}

		bRet = true;
	} while (false);
	return bRet;
}



bool CEbootObject::IsLoadableSgmt(Elf64_Phdr& phdr)
{
	if (phdr.p_type == PT_LOAD)
	{
		return true;
	}

	if (phdr.p_type == PT_SCE_RELRO)
	{
		return true;
	}

	return false;
}

uint CEbootObject::GetLoadableSize()
{
	size_t nLoadStart = (size_t)(-1);
	size_t nLoadEnd = 0;

	for (Elf64_Phdr& phdr : m_vtPhdrs)
	{
		if (!IsLoadableSgmt(phdr))
		{
			continue;
		}

		if (phdr.p_vaddr < nLoadStart)
		{
			nLoadStart = phdr.p_vaddr;
		}

		size_t nAligned = ALIGN_DOWN(phdr.p_vaddr + phdr.p_memsz, phdr.p_align);
		if (nAligned > nLoadEnd)
		{
			nLoadEnd = nAligned;
		}
	}

	return (nLoadEnd - nLoadStart);
}


bool CEbootObject::LoadInVitrualMemory()
{
	bool bRet = false;
	do 
	{
		uint nLoadSize = GetLoadableSize();
		if (!nLoadSize)
		{
			break;
		}

		m_stModuleInfo.pMappedAddr = (byte*)UtilMemory::VMMap(nLoadSize, 
			UtilMemory::VMPF_READ | UtilMemory::VMPF_EXECUTE);
		if (!m_stModuleInfo.pMappedAddr)
		{
			break;
		}
		m_stModuleInfo.nMappedSize = nLoadSize;

		for (Elf64_Phdr& phdr : m_vtPhdrs)
		{
			if (phdr.p_flags & PF_X)
			{
				bRet = LoadSgmtCode(phdr);
				continue;
			}
			
			if (phdr.p_type == PT_SCE_RELRO)
			{
				bRet = LoadSgmtSceRelro(phdr);
				continue;
			}

			if (phdr.p_flags & PF_W)
			{
				bRet = LoadSgmtData(phdr);
				break;
			}
		}

		bRet = true;
	} while (false);
	return bRet;
}

bool CEbootObject::LoadSgmtCode(Elf64_Phdr& phdr)
{
	bool bRet = false;
	do 
	{
		if (m_vtFileMem.empty())
		{
			break;
		}

		m_stModuleInfo.nCodeSize = phdr.p_memsz;
		m_stModuleInfo.pCodeAddr = (byte*)ALIGN_DOWN((size_t)m_stModuleInfo.pMappedAddr + phdr.p_vaddr, phdr.p_align);
		byte* pCodeFileOff = m_vtFileMem.data() + phdr.p_offset;

		memcpy(m_stModuleInfo.pCodeAddr, pCodeFileOff, phdr.p_filesz);

		//
		m_stModuleInfo.pEntryPoint = m_stModuleInfo.pCodeAddr + m_stEhdr.e_entry;

		//
		m_stModuleInfo.pTlsAddr = m_stModuleInfo.pCodeAddr + (uint64)m_stModuleInfo.pTlsAddr;
	
		bRet = true;
	} while (false);
	return bRet;
}

bool CEbootObject::LoadSgmtSceRelro(Elf64_Phdr& phdr)
{
	bool bRet = false;
	do 
	{
		if (m_vtFileMem.empty())
		{
			break;
		}

		byte* pRelroAddr = (byte*)ALIGN_DOWN((size_t)m_stModuleInfo.pMappedAddr + phdr.p_vaddr, phdr.p_align);
		byte* pRelroFileOff = m_vtFileMem.data() + phdr.p_offset;
		memcpy(pRelroAddr, pRelroFileOff, phdr.p_filesz);

		bRet = true;
	} while (false);
	return bRet;
}

bool CEbootObject::LoadSgmtData(Elf64_Phdr& phdr)
{
	bool bRet = false;
	do
	{
		if (m_vtFileMem.empty())
		{
			break;
		}

		m_stModuleInfo.nDataSize = phdr.p_memsz;
		m_stModuleInfo.pDataAddr = (byte*)ALIGN_DOWN((size_t)m_stModuleInfo.pMappedAddr + phdr.p_vaddr, phdr.p_align);
		if (m_stModuleInfo.pDataAddr < m_stModuleInfo.pCodeAddr + m_stModuleInfo.nCodeSize)
		{
			LOG_ERR("data segment is in code segment.");
			break;
		}

		byte* pDataFileOff = m_vtFileMem.data() + phdr.p_offset;
		memcpy(m_stModuleInfo.pDataAddr, pDataFileOff, phdr.p_filesz);

		bRet = true;
	} while (false);
	return bRet;
}

bool CEbootObject::Relocate()
{
	return RelocateRela() && RelocatePltRela();
	//return RelocatePltRela();
}

bool CEbootObject::RelocateRela()
{
	bool bRet = false;
	do 
	{
		if (m_vtFileMem.empty())
		{
			break;
		}
		
		byte* pImageBase = m_stModuleInfo.pCodeAddr;
		byte* pStrTab = m_stModuleInfo.pStrTab;
		Elf64_Sym* pSymTab = (Elf64_Sym*)m_stModuleInfo.pSymTab;
		Elf64_Rela* pRelaEntries = (Elf64_Rela*)m_stModuleInfo.pRela;
		for (uint i = 0; i != m_stModuleInfo.nRelaCount; ++i)
		{
			Elf64_Rela* pRela = &pRelaEntries[i];
			auto nType = ELF64_R_TYPE(pRela->r_info);
			auto nSymIdx = ELF64_R_SYM(pRela->r_info);

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
				Elf64_Sym& symbol = pSymTab[nSymIdx];
				auto nBinding = ELF64_ST_BIND(symbol.st_info);
				uint64 nSymVal = 0;

				if (nBinding == STB_LOCAL)
				{
					nSymVal = (uint64)(pImageBase + symbol.st_value);
				}
				else if (nBinding == STB_GLOBAL || nBinding == STB_WEAK)
				{
					char* pName = (char*)&pStrTab[symbol.st_name];
					if (!ResolveSymbol(pName, nSymVal))
					{
						LOG_ERR("can not get symbol address.");
						break;
					}
				}
				else
				{
					LOG_ERR("invalid sym bingding %d", nBinding);
				}

				*(uint64*)&pImageBase[pRela->r_offset] = nSymVal + pRela->r_addend;
			}
				break;
			case R_X86_64_RELATIVE:
			{
				*(uint64*)&pImageBase[pRela->r_offset] = (uint64)(pImageBase + pRela->r_addend);
			}
				break;
			default:
				LOG_FIXME("rela type not handled %d", nType);
				break;
			}
		}
		bRet = true;
	} while (false);
	return bRet;
}

bool CEbootObject::RelocatePltRela()
{
	bool bRet = false;
	do
	{
		if (m_vtFileMem.empty())
		{
			break;
		}

		byte* pImageBase = m_stModuleInfo.pCodeAddr;
		byte* pStrTab = m_stModuleInfo.pStrTab;;
		Elf64_Sym* pSymTab = (Elf64_Sym*)m_stModuleInfo.pSymTab;
		Elf64_Rela* pRelaEntries = (Elf64_Rela*)m_stModuleInfo.pPltRela;
		for (uint i = 0; i != m_stModuleInfo.nPltRelaCount; ++i)
		{
			Elf64_Rela* pRela = &pRelaEntries[i];
			auto nType = ELF64_R_TYPE(pRela->r_info);
			auto nSymIdx = ELF64_R_SYM(pRela->r_info);

			switch (nType)
			{
			case R_X86_64_JUMP_SLOT:
			{
				Elf64_Sym& symbol = pSymTab[nSymIdx];
				auto nBinding = ELF64_ST_BIND(symbol.st_info);
				uint64 nSymVal = 0;

				if (nBinding == STB_LOCAL)
				{
					nSymVal = (uint64)(pImageBase + symbol.st_value);
				}
				else if (nBinding == STB_GLOBAL || nBinding == STB_WEAK)
				{
					char* pName = (char*)&pStrTab[symbol.st_name];
					if (!ResolveSymbol(pName, nSymVal))
					{
						LOG_ERR("can not get symbol address.");
						break;
					}
				}
				else
				{
					LOG_ERR("invalid sym bingding %d", nBinding);
				}

				*(uint64*)&pImageBase[pRela->r_offset] = nSymVal;
			}
				break;
			default:
				LOG_FIXME("rela type not handled %d", nType);
				break;
			}
		}
		bRet = true;
	} while (false);
	return bRet;
}

bool CEbootObject::ResolveSymbol(const std::string& strEncName, uint64& nAddr)
{
	bool bRet = false;
	do 
	{
		if (!m_pLinker)
		{
			break;
		}

		LOG_DEBUG("Enc Name: %s", strEncName.c_str());
		uint nModId = 0;
		uint nLibId = 0;
		uint64 nNid = 0;
		if (!DecodeEncName(strEncName, nModId, nLibId, nNid))
		{
			break;
		}

		std::string modName = ModuleNameFromId(nModId);
		std::string libName = LibraryNameFromId(nLibId);
		if (modName.empty() || libName.empty())
		{
			LOG_ERR("can not find module %d or libray %d", nModId, nLibId);
			break;
		}

		LOG_TRACE("resolve symbol %s %s %llX", modName.c_str(), libName.c_str(), nNid);

		void* pSymAddr = NULL;
		if (!m_pLinker->ResolveSymbol(modName, libName, nNid, &pSymAddr))
		{
			LOG_ERR("can not resolve symbol %s %s %llX", modName.c_str(), libName.c_str(), nNid);
			break;
		}

		nAddr = (uint64)pSymAddr;

		bRet = true;
	} while (false);
	return bRet;
}

bool CEbootObject::DecodeEncName(const std::string& strEncName, uint& nModuleId, uint& nLibraryId, uint64& nNid)
{
	bool bRet = false;
	do 
	{
		std::vector<std::string> vtNameParts;
		if (!UtilString::Split(strEncName, '#', vtNameParts))
		{
			break;
		}

		if (!DecodeValue(vtNameParts[0], nNid))
		{
			break;
		}

		uint64 nLibId = 0;
		if (!DecodeValue(vtNameParts[1], nLibId))
		{
			break;
		}
		nLibraryId = static_cast<uint>(nLibId);

		uint64 nModId = 0;
		if (!DecodeValue(vtNameParts[2], nModId))
		{
			break;
		}
		nModuleId = static_cast<uint>(nModId);

		bRet = true;
	} while (false);
	return bRet;
}


/*

from orbis-ld.exe (sdk 4.500)
sub_1400B8AB0

void Encode(uint64 nVal)
{
	const char pCodes[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
	char encoded[11] = { 0 };
	encoded[0] = pCodes[nVal >> 58];
	encoded[1] = pCodes[(nVal >> 52) & 0x3F];
	encoded[2] = pCodes[(nVal >> 46) & 0x3F];
	encoded[3] = pCodes[(nVal >> 40) & 0x3F];
	encoded[4] = pCodes[(nVal >> 34) & 0x3F];
	encoded[5] = pCodes[(nVal >> 28) & 0x3F];
	encoded[6] = pCodes[(nVal >> 22) & 0x3F];
	encoded[7] = pCodes[(nVal >> 16) & 0x3F];
	encoded[8] = pCodes[(nVal >> 10) & 0x3F];
	encoded[9] = pCodes[(nVal >> 4) & 0x3F];
	encoded[10] = pCodes[4 * (nVal & 0xF)];
}
*/

bool CEbootObject::DecodeValue(const std::string& strEnc, uint64& nVal)
{
	bool bRet = false;

	//the max length for an encode id is 11
	//from orbis-ld.exe
	const uint nEncLenMax = 11;
	const char pCodes[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";

	do 
	{

		if (strEnc.size() > nEncLenMax)
		{
			LOG_ERR("encode id too long: %s", strEnc.c_str());
			break;
		}

		bool bError = false;
		nVal = 0;

		for (int i = 0; i < strEnc.size(); ++i)
		{
			auto pChPos = strchr(pCodes, strEnc[i]);
			uint nIndex = 0;

			if (pChPos != nullptr)
			{
				nIndex = static_cast<uint>(pChPos - pCodes);
			}
			else
			{
				bError = true;
				break;
			}

			// NID is 64 bits long, thus we do 6 x 10 + 4 times
			if (i < nEncLenMax - 1)
			{
				nVal <<= 6;
				nVal |= nIndex;
			}
			else
			{
				nVal <<= 4;
				nVal |= (nIndex >> 2);
			}
		}

		if (bError)
		{
			break;
		}

		bRet = true;
	} while (false);
	return bRet;
}

std::string CEbootObject::ModuleNameFromId(uint nModId)
{
	std::string strModName;
	do 
	{
		auto iter = std::find_if(m_vtModules.begin(), m_vtModules.end(),
			[nModId](IMPORT_MODULE& stMod) { return nModId == stMod.id; });

		if (iter == m_vtModules.end())
		{
			break;
		}

		strModName = iter->strName;
	} while (false);
	return strModName;
}

std::string CEbootObject::LibraryNameFromId(uint nLibId)
{
	std::string strLibName;
	do
	{
		auto iter = std::find_if(m_vtLibraries.begin(), m_vtLibraries.end(),
			[nLibId](IMPORT_LIBRARY& stLib) { return nLibId == stLib.id; });

		if (iter == m_vtLibraries.end())
		{
			break;
		}

		strLibName = iter->strName;
	} while (false);
	return strLibName;
}
#endif
