#include <fstream>
#include <hack/Parser.hpp>
#include <hack/Assembler.hpp>

#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char const *argv[])
{
    std::string fileName = argv[1];

    std::ifstream ifs(fileName, std::ifstream::in);
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string test(ss.str());
    ifs.close();

    const auto instructions = hack::Parser::parse(ss);

    hack::Assembler assembler;
    const auto binary = assembler.assemble(instructions);


    std::ofstream ofs("output.txt");
    ofs << binary.rdbuf();
    ofs.close();
}
