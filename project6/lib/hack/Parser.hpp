#pragma once

#include "Code.hpp"

#include <sstream>

namespace hack
{
    class Parser
    {
        public:
            static Instructions parse(std::stringstream& is);

        private:
            static Instruction parseLabel(const std::string& line);
            static Instruction parseAInstruction(const std::string& line);
            static Instruction parseCInstruction(const std::string& line);
    };
}