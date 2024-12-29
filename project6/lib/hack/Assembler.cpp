#include "Assembler.hpp"
#include "hack/Code.hpp"
#include <cstdint>
#include <sstream>
#include <variant>

namespace hack
{
    Assembler::Assembler()
    {
        for (auto i = 0; i < 16; ++i)
        {
            symbolTable.addSymbol("R"+std::to_string(i), i);
        }

        symbolTable.addSymbol("SP", 0);
        symbolTable.addSymbol("LCL", 1);
        symbolTable.addSymbol("ARG", 2);
        symbolTable.addSymbol("THIS", 3);
        symbolTable.addSymbol("THAT", 4);

        symbolTable.addSymbol("SCREEN", 16384);
        symbolTable.addSymbol("KBD", 24576);
    }

    std::stringstream Assembler::assemble(const Instructions& instructions)
    {
        firstPass(instructions);

        std::stringstream ss;
        for (const auto& instr : instructions)
        {
            if (const auto* aInstr = std::get_if<AInstruction>(&instr))
            {
                ss << toBinary(*aInstr);
                ss << "\n";
            }
            else if (const auto* cInstr = std::get_if<CInstruction>(&instr))
            {
                ss << toBinary(*cInstr);
                ss << "\n";
            }
        }

        return ss;
    }

    void Assembler::firstPass(const Instructions& instructions)
    {
        uint16_t pc = 0;
        for (const auto& instr : instructions)
        {
            if (const auto * label = std::get_if<Label>(&instr))
            {
                symbolTable.addSymbol(label->label, pc);
            }
            else
            {
                ++pc;
            }
        }

        for (const auto& instr : instructions)
        {
            if (const auto* aInstr = std::get_if<AInstruction>(&instr))
            {
                if (const auto * variable = std::get_if<std::string>(&aInstr->value))
                {
                    if (!symbolTable.hasSymbol(*variable))
                    {
                        symbolTable.addSymbol(*variable, nextMemoryAddress);
                        ++nextMemoryAddress;
                    }
                }
            }
        }
    }

    std::string Assembler::toBinary(const AInstruction& instruction) const
    {
        std::stringstream ss;

        ss << "0";
        
        if (const auto* value = std::get_if<uint16_t>(&instruction.value))
        {
            ss << std::bitset<15>(*value);
        }
        else if (const auto* variable = std::get_if<std::string>(&instruction.value))
        {
            ss << std::bitset<15>(symbolTable.getAddress(*variable));
        }

        return ss.str();
    }

    std::string Assembler::toBinary(const CInstruction& instruction) const
    {
        std::stringstream ss;

        using DestBits = std::bitset<3>;
        DestBits destBits;
        if      (instruction.dest == "D")   { destBits = DestBits("010"); }
        else if (instruction.dest == "MD")  { destBits = DestBits("011"); }
        else if (instruction.dest == "M")   { destBits = DestBits("001"); }
        else if (instruction.dest == "AMD") { destBits = DestBits("111"); }
        else if (instruction.dest == "AM")  { destBits = DestBits("101"); }
        else if (instruction.dest == "AD")  { destBits = DestBits("110"); }
        else if (instruction.dest == "A")   { destBits = DestBits("100"); }

        using CompBits = std::bitset<6>;
        CompBits compBits;
        bool useMemory = false;
        if      (instruction.comp == "D+1") { compBits = CompBits("011111"); }
        else if (instruction.comp == "A+1") { compBits = CompBits("110111"); }
        else if (instruction.comp == "M+1") { compBits = CompBits("110111"); useMemory = true; }
        else if (instruction.comp == "D-1") { compBits = CompBits("001110"); }
        else if (instruction.comp == "A-1") { compBits = CompBits("110010"); }
        else if (instruction.comp == "M-1") { compBits = CompBits("110010"); useMemory = true; }
        else if (instruction.comp == "D+A") { compBits = CompBits("000010"); }
        else if (instruction.comp == "D+M") { compBits = CompBits("000010"); useMemory = true; }
        else if (instruction.comp == "D-A") { compBits = CompBits("010011"); }
        else if (instruction.comp == "D-M") { compBits = CompBits("010011"); useMemory = true; }
        else if (instruction.comp == "A-D") { compBits = CompBits("000111"); }
        else if (instruction.comp == "M-D") { compBits = CompBits("000111"); useMemory = true; }
        else if (instruction.comp == "D&A") { compBits = CompBits("000000"); }
        else if (instruction.comp == "D&M") { compBits = CompBits("000000"); useMemory = true; }
        else if (instruction.comp == "D|A") { compBits = CompBits("010101"); }
        else if (instruction.comp == "D|M") { compBits = CompBits("010101"); useMemory = true; }
        else if (instruction.comp == "0")   { compBits = CompBits("101010"); }
        else if (instruction.comp == "1")   { compBits = CompBits("111111"); }
        else if (instruction.comp == "-1")  { compBits = CompBits("111010"); }
        else if (instruction.comp == "D")   { compBits = CompBits("001100"); }
        else if (instruction.comp == "A")   { compBits = CompBits("110000"); }
        else if (instruction.comp == "M")   { compBits = CompBits("110000"); useMemory = true; }
        else if (instruction.comp == "!D")  { compBits = CompBits("001101"); }
        else if (instruction.comp == "!A")  { compBits = CompBits("110001"); }
        else if (instruction.comp == "!M")  { compBits = CompBits("110001"); useMemory = true; }
        else if (instruction.comp == "-D")  { compBits = CompBits("001111"); }
        else if (instruction.comp == "-A")  { compBits = CompBits("110011"); }
        else if (instruction.comp == "-M")  { compBits = CompBits("110011"); useMemory = true; }

        using JumpBits = std::bitset<3>;
        JumpBits jumpBits;
        if      (instruction.jump == "JGT") { jumpBits = JumpBits("001"); }
        else if (instruction.jump == "JEQ") { jumpBits = JumpBits("010"); }
        else if (instruction.jump == "JGE") { jumpBits = JumpBits("011"); }
        else if (instruction.jump == "JLT") { jumpBits = JumpBits("100"); }
        else if (instruction.jump == "JNE") { jumpBits = JumpBits("101"); }
        else if (instruction.jump == "JLE") { jumpBits = JumpBits("110"); }
        else if (instruction.jump == "JMP") { jumpBits = JumpBits("111"); }

        ss << "111";
        ss << std::bitset<1>(useMemory);
        ss << compBits;
        ss << destBits;
        ss << jumpBits;
        return ss.str();
    }
}