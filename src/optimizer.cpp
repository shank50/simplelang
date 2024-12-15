#include "optimizer.h"

#include <cmath>
#include <limits>
#include <memory>
#include <stdexcept>

static ExprPtr optimizeExpr(ExprPtr expr);

static bool isNumber(ExprPtr& expr, double& value) {
    NumberExpr* number = dynamic_cast<NumberExpr*>(expr.get());
    if (number) {
        value = number->value;
        return true;
    }
    return false;
}

static bool isPowerOfTwo(double value, int& exponent) {
    double rounded = std::round(value);
    if (std::fabs(value - rounded) > 1e-9) {
        return false;
    }
    long long intValue = static_cast<long long>(rounded);
    if (intValue <= 0) {
        return false;
    }
    if ((intValue & (intValue - 1)) != 0) {
        return false;
    }
    exponent = 0;
    while (intValue > 1) {
        intValue >>= 1;
        ++exponent;
    }
    return true;
}

static ExprPtr optimizeExpr(ExprPtr expr) {
    BinaryExpr* binary = dynamic_cast<BinaryExpr*>(expr.get());
    if (binary) {
        binary->left = optimizeExpr(std::move(binary->left));
        binary->right = optimizeExpr(std::move(binary->right));

        double leftValue = 0.0;
        double rightValue = 0.0;
        bool leftIsNumber = isNumber(binary->left, leftValue);
        bool rightIsNumber = isNumber(binary->right, rightValue);

        if (leftIsNumber && rightIsNumber) {
            double result = 0.0;
            switch (binary->operation) {
            case BinaryOp::Add:
                result = leftValue + rightValue;
                break;
            case BinaryOp::Sub:
                result = leftValue - rightValue;
                break;
            case BinaryOp::Mul:
                result = leftValue * rightValue;
                break;
            case BinaryOp::Div:
                if (std::fabs(rightValue) < 1e-9) {
                    throw std::runtime_error("Division by zero in constant expression");
                }
                result = leftValue / rightValue;
                break;
            case BinaryOp::Shl: {
                long long lhs = static_cast<long long>(std::llround(leftValue));
                long long rhs = static_cast<long long>(std::llround(rightValue));
                result = static_cast<double>(lhs << rhs);
                break;
            }
            }
            return std::make_unique<NumberExpr>(result);
        }

        if (binary->operation == BinaryOp::Mul) {
            int exponent = 0;
            if (rightIsNumber && isPowerOfTwo(rightValue, exponent)) {
                binary->operation = BinaryOp::Shl;
                binary->right = std::make_unique<NumberExpr>(static_cast<double>(exponent));
            } else if (leftIsNumber && isPowerOfTwo(leftValue, exponent)) {
                binary->operation = BinaryOp::Shl;
                binary->left.swap(binary->right);
                binary->right = std::make_unique<NumberExpr>(static_cast<double>(exponent));
            }
        }

        return expr;
    }
    return expr;
}

void optimizeProgram(Program& program) {
    for (size_t i = 0; i < program.size(); ++i) {
        LetStmt* letStmt = dynamic_cast<LetStmt*>(program[i].get());
        if (letStmt) {
            letStmt->value = optimizeExpr(std::move(letStmt->value));
            continue;
        }

        PrintStmt* printStmt = dynamic_cast<PrintStmt*>(program[i].get());
        if (printStmt) {
            printStmt->value = optimizeExpr(std::move(printStmt->value));
        }
    }
}
