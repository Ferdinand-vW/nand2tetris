#include "SymbolTable.hpp"
#include <string_view>

namespace hack
{
    bool SymbolTable::hasSymbol(std::string_view symbol) const
    {
        return symbols.contains(symbol.data());
    }

    void SymbolTable::addSymbol(std::string_view symbol, uint16_t address)
    {
        symbols.emplace(symbol.data(), address);
    }


    uint16_t SymbolTable::getAddress(std::string_view symbol) const
    {
        auto it = symbols.find(symbol.data());
        if (it != symbols.end())
        {
            return it->second;
        }
        else
        {
            return 0;
        }
    }
}