#pragma once

#include "GPCS4Common.h"
#include "Emulator/Module.h"
#include "elf.h"

#include <cstdio>
#include <memory>
#include <vector>
#include <string>

class ELFMapper
{
public:
	ELFMapper() = default;

	ELFMapper(ELFMapper const &) = delete;
	void operator=(ELFMapper const &) = delete;

	bool loadFile(std::string const &filePath, MemoryMappedModule *mod);
	bool validateHeader();
	bool parseSegmentHeaders();
	bool parseDynamicSection();
	bool mapImageIntoMemroy();
	bool parseSymbols();

	bool decodeValue(std::string const &strEnc, uint64_t &val);
	bool decodeEncodedName(std::string const &strEncName,
						   uint *nModuleId,
						   uint *nLibraryId,
						   uint64_t *nNid);

private:
	bool prepareTables(Elf64_Dyn const &entry, uint index);
	bool parseSingleDynEntry(Elf64_Dyn const &entry, uint index);
	size_t calculateTotalLoadableSize();
	bool isSegmentLoadable(Elf64_Phdr const &hdr);

	bool mapCodeSegment(Elf64_Phdr const &hdr);
	bool mapSecReloSegment(Elf64_Phdr const &phdr);
	bool mapDataSegment(Elf64_Phdr const &phdr);

	MemoryMappedModule *m_moduleData;
};
