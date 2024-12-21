#pragma once

#include "bytecode.h"

class VM {
public:
    void execute(const Bytecode& code);
};
