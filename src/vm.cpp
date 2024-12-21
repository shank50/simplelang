#include "vm.h"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

static double popValue(std::vector<double>& stack) {
    if (stack.empty()) {
        throw std::runtime_error("VM stack underflow");
    }
    double value = stack.back();
    stack.pop_back();
    return value;
}

static void printValue(double value) {
    double rounded = std::round(value);
    if (std::fabs(value - rounded) < 1e-9) {
        std::cout << static_cast<long long>(std::llround(rounded)) << '\n';
    } else {
        std::cout << value << '\n';
    }
}

void VM::execute(const Bytecode& code) {
    std::vector<double> stack;
    std::unordered_map<std::string, double> variables;

    for (size_t i = 0; i < code.size(); ++i) {
        Instruction instr = code[i];
        switch (instr.op) {
        case OpCode::PushConst:
            stack.push_back(instr.numberValue);
            break;
        case OpCode::LoadVar: {
            std::unordered_map<std::string, double>::iterator it = variables.find(instr.stringValue);
            if (it == variables.end()) {
                throw std::runtime_error("Undefined variable: " + instr.stringValue);
            }
            stack.push_back(it->second);
            break;
        }
        case OpCode::StoreVar: {
            double value = popValue(stack);
            variables[instr.stringValue] = value;
            break;
        }
        case OpCode::Add: {
            double rhs = popValue(stack);
            double lhs = popValue(stack);
            stack.push_back(lhs + rhs);
            break;
        }
        case OpCode::Sub: {
            double rhs = popValue(stack);
            double lhs = popValue(stack);
            stack.push_back(lhs - rhs);
            break;
        }
        case OpCode::Mul: {
            double rhs = popValue(stack);
            double lhs = popValue(stack);
            stack.push_back(lhs * rhs);
            break;
        }
        case OpCode::Div: {
            double rhs = popValue(stack);
            double lhs = popValue(stack);
            if (std::fabs(rhs) < 1e-9) {
                throw std::runtime_error("Division by zero");
            }
            stack.push_back(lhs / rhs);
            break;
        }
        case OpCode::Shl: {
            double rhs = popValue(stack);
            double lhs = popValue(stack);
            long long lhsInt = static_cast<long long>(std::llround(lhs));
            long long shift = static_cast<long long>(std::llround(rhs));
            if (shift < 0) {
                throw std::runtime_error("Negative shift amount");
            }
            stack.push_back(static_cast<double>(lhsInt << shift));
            break;
        }
        case OpCode::Print: {
            double value = popValue(stack);
            printValue(value);
            break;
        }
        }
    }
}
