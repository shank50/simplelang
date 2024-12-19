#pragma once

#include "ast.h"
#include "bytecode.h"

class CodeGenerator {
public:
    Bytecode generate(const Program& program);

private:
    void emitStatement(const Stmt* stmt);
    void emitExpression(const Expr* expr);

    Bytecode m_code;
};
