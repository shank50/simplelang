#pragma once

#include <string>
#include <vector>

enum class OpCode {
    PushConst,
    LoadVar,
    StoreVar,
    Add,
    Sub,
    Mul,
    Div,
    Shl,
    Print
};

struct Instruction {
    OpCode op;
    double numberValue = 0.0;
    std::string stringValue;
    int intValue = 0;
};

using Bytecode = std::vector<Instruction>;
