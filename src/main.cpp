#include "codegen.h"
#include "lexer.h"
#include "optimizer.h"
#include "parser.h"
#include "vm.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: simplelang <filename>\n";
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "Failed to open file: " << argv[1] << "\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << input.rdbuf();

    try {
    Lexer lexer(buffer.str());
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
        Program program = parser.parse();

        optimizeProgram(program);

        CodeGenerator generator;
        Bytecode bytecode = generator.generate(program);

        VM vm;
        vm.execute(bytecode);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
