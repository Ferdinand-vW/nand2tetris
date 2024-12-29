#include "Parser.hpp"
#include "hack/Code.hpp"

#include <cctype>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

namespace hack
{
    Instructions Parser::parse(std::stringstream &is)
    {
        Instructions instructions;
        std::string contents(is.str());
        while(!is.eof())
        {

            std::string str;
            std::getline(is, str, '\n');
            std::erase_if(str, [](auto c) { return c == ' ' || c == '\t' || c == '\r'; });

            if (str.starts_with("//") || str.starts_with("\r"))
            {
                continue;
            }

            if (str.starts_with("("))
            {
                instructions.emplace_back(Parser::parseLabel(str));
            }
            else if (str.starts_with("@"))
            {
                instructions.emplace_back(Parser::parseAInstruction(str));
            }
            else if (!str.empty())
            {
                instructions.emplace_back(Parser::parseCInstruction(str));
            }
        }

        return instructions;
    }

    Instruction Parser::parseLabel(const std::string& line)
    {
        std::string label = line;

        std::erase_if(label, [](auto c) { return c == '(' || c == ')'; });

        return Label{label};
    }

    Instruction Parser::parseAInstruction(const std::string& line)
    {
        // remove @
        std::string aLabel = line.substr(1, line.size() - 1);
        
        if (!aLabel.empty() && std::isdigit(aLabel[0]))
        {
            uint16_t address = std::atoi(aLabel.c_str());
            return AInstruction{address};
        }

        return AInstruction{aLabel};
    }

    // dest=comp;jump
    Instruction Parser::parseCInstruction(const std::string& line)
    {
        std::string str = line;

        std::string dest;
        std::string comp;
        std::string jump;

        auto i = str.find_first_of("=");
        if (i != std::string::npos)
        {
            dest = str.substr(0, i);
            str.erase(0, i+1);
        }

        auto j = str.find_first_of(";");

        if (j != std::string::npos)
        {
            jump = str.substr(j+1, str.size());
        }
        else
        {
            j = str.size();
        }

        comp = str.substr(0, j);

        return CInstruction{dest, comp, jump};
    }
}