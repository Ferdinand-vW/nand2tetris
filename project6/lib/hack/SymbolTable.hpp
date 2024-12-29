#pragma once

#include <unordered_map>
#include <string>

namespace hack
{
    class SymbolTable
    {
        public:
            bool hasSymbol(std::string_view) const;
            void addSymbol(std::string_view, uint16_t);
            uint16_t getAddress(std::string_view) const;

        private:
            std::unordered_map<std::string, uint16_t> symbols;
    };
}