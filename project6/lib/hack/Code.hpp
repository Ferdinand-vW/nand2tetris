#pragma once

#include <bitset>
#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace hack
{
    struct AInstruction
    {
        using Value = std::variant<uint16_t, std::string>;

        Value value;
    };

    struct CInstruction
    {
        std::string dest;
        std::string comp;
        std::string jump;
    };

    struct Label
    {
        std::string label;
    };

    using Instruction = std::variant<CInstruction, AInstruction, Label>;
    using Instructions = std::vector<Instruction>;
}