#include "codegen.h"

#include <stdexcept>

Bytecode CodeGenerator::generate(const Program& program) {
    m_code.clear();
    for (size_t i = 0; i < program.size(); ++i) {
        emitStatement(program[i].get());
    }
    return m_code;
}

void CodeGenerator::emitStatement(const Stmt* stmt) {
    const LetStmt* letStmt = dynamic_cast<const LetStmt*>(stmt);
    if (letStmt) {
        emitExpression(letStmt->value.get());
        Instruction instr;
        instr.op = OpCode::StoreVar;
        instr.stringValue = letStmt->name;
        m_code.push_back(instr);
        return;
    }
    const PrintStmt* printStmt = dynamic_cast<const PrintStmt*>(stmt);
    if (printStmt) {
        emitExpression(printStmt->value.get());
        Instruction instr;
        instr.op = OpCode::Print;
        m_code.push_back(instr);
        return;
    }
    throw std::runtime_error("Unknown statement type during code generation");
}

void CodeGenerator::emitExpression(const Expr* expr) {
    const NumberExpr* number = dynamic_cast<const NumberExpr*>(expr);
    if (number) {
        Instruction instr;
        instr.op = OpCode::PushConst;
        instr.numberValue = number->value;
        m_code.push_back(instr);
        return;
    }
    const VariableExpr* variable = dynamic_cast<const VariableExpr*>(expr);
    if (variable) {
        Instruction instr;
        instr.op = OpCode::LoadVar;
        instr.stringValue = variable->name;
        m_code.push_back(instr);
        return;
    }
    const BinaryExpr* binary = dynamic_cast<const BinaryExpr*>(expr);
    if (binary) {
        emitExpression(binary->left.get());
        emitExpression(binary->right.get());
        OpCode op = OpCode::Add;
        switch (binary->operation) {
        case BinaryOp::Add:
            op = OpCode::Add;
            break;
        case BinaryOp::Sub:
            op = OpCode::Sub;
            break;
        case BinaryOp::Mul:
            op = OpCode::Mul;
            break;
        case BinaryOp::Div:
            op = OpCode::Div;
            break;
        case BinaryOp::Shl:
            op = OpCode::Shl;
            break;
        }
        Instruction instr;
        instr.op = op;
        m_code.push_back(instr);
        return;
    }
    throw std::runtime_error("Unknown expression type during code generation");
}
