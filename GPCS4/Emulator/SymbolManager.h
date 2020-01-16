#include <string>
#include <unordered_map>

class SymbolManager
{
public:
	const void *findNativeSymbol(std::string const &modName,
								 std::string const &libName,
								 uint64_t nid) const;

	const void *findNativeSymbol(std::string const &modName,
		                   std::string const &libName,
		                   std::string const &symbName) const;

	const void *findBuiltinSymbol(std::string const &modName,
		                    std::string const &libName,
                            uint64_t nid) const;

	const void *findBuiltinSymbol(std::string const &modName,
		                    std::string const &libName,
                            std::string const &name) const;

	bool registerNativeSymbol(std::string const &modName,
		                      std::string const &libName,
		                      uint64_t nid,
		                      const void *address);

	bool registerNativeSymbol(std::string const &modName,
		                      std::string const &libName,
		                      std::string const &name,
		                      const void *address);

	bool registerBuiltinSymbol(std::string const &modName,
		                       std::string const &libName,
                               uint64_t nid,
		                       const void *address);

	bool registerBuiltinSymbol(std::string const &modName,
		                       std::string const &libName,
                               std::string const &name,
		                       const void *address);

private:
	using NidAddrMap = std::unordered_map<uint64_t, const void*>;
	using NameAddrMap = std::unordered_map<std::string, const void*>;
	using LibSymbNidMap = std::unordered_map<std::string, NidAddrMap>;
	using LibSymbNameMap = std::unordered_map<std::string, NameAddrMap>;
	using ModSymbNameMap = std::unordered_map<std::string, LibSymbNameMap>;
	using ModSymbNidMap = std::unordered_map<std::string, LibSymbNidMap>;

	template<typename Table, typename KeyType>
	const void *findSymbolGeneric(Table &table,
		                    std::string const &modName,
		                    std::string const &libName,
		                    KeyType nid) const
	{
		auto &tab = table;
		const void *address = nullptr;

		do
		{
			auto modIter = tab.find(modName);
			if (modIter == tab.end())
			{
				address = nullptr;
				break;
			}

			auto libIter = modIter->second.find(libName);
			if (libIter == modIter->second.end())
			{
				address = nullptr;
				break;
			}
			
			auto nidIter = libIter->second.find(nid);
			if (nidIter == libIter->second.end())
			{
				address = nullptr;
				break;
			}

			address = nidIter->second;
		} while (false);

		return address;
	}

	template<typename Table, typename KeyType>
	bool registerSymbolNidGeneric(Table &table,
							   std::string const &modName,
							   std::string const &libName,
							   KeyType nid,
							   const void *addr)
	{
		using LibMapType = typename Table::mapped_type;
		using AddrMapType = typename Table::mapped_type::mapped_type;

		bool ret     = false;

		auto& tab = table;
		auto modIter = tab.find(modName);
		if (modIter == tab.end())
		{
			typename Table::mapped_type libMap;
			tab.emplace(std::make_pair(modName, LibMapType{}));
			modIter = tab.find(modName);
		}
		
		auto libIter = modIter->second.find(libName);
		if (libIter == modIter->second.end())
		{
			modIter->second.emplace(std::make_pair(libName, AddrMapType{}));
			libIter = modIter->second.find(libName);
		}

		auto symbIter = libIter->second.find(nid);
		if (symbIter == libIter->second.end())
		{
			libIter->second.emplace(std::make_pair(nid, addr));
			ret = true;
		}
		else
		{
			// already registered
			ret = false;			
		}

		return ret;
	}

	ModSymbNidMap m_nativeModuleSymbolNidDir;
	ModSymbNidMap m_builtinModuleSymbolNidDir;

	ModSymbNameMap m_nativeModuleSymbolNameDir;
	ModSymbNameMap m_builtinModuleSymbolNameDir;
};
