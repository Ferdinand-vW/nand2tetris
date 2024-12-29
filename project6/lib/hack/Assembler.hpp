#pragma once

#include "SymbolTable.hpp"
#include "Code.hpp"
#include <sstream>

namespace hack
{
    class Assembler
    {
        public:
            Assembler();
        
            std::stringstream assemble(const Instructions& instructions);

        private:
            void firstPass(const Instructions& instructions);

            std::string toBinary(const CInstruction& instruction) const;
            std::string toBinary(const AInstruction& instruction) const;

        private:
        SymbolTable symbolTable;
        uint16_t nextMemoryAddress = 16;
    };
}