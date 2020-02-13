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

	bool loadFile(std::string const &filePath, NativeModule *mod);
	bool validateHeader();
	bool parseSegmentHeaders();
	bool parseDynamicSection();
	bool mapImageIntoMemory();
	bool parseSymbols();

	void *getProcParam() const;

private:
	bool prepareTables(Elf64_Dyn const &entry, uint32_t index);
	bool parseSingleDynEntry(Elf64_Dyn const &entry, uint32_t index);
	size_t calculateTotalLoadableSize();
	bool isSegmentLoadable(Elf64_Phdr const &hdr);

	bool mapCodeSegment(Elf64_Phdr const &hdr);
	bool mapSecReloSegment(Elf64_Phdr const &phdr);
	bool mapDataSegment(Elf64_Phdr const &phdr);

	NativeModule *m_moduleData;
};
