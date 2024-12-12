#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

enum class BinaryOp {
    Add,
    Sub,
    Mul,
    Div,
    Shl
};

struct Expr {
    virtual ~Expr() = default;
};

struct NumberExpr : Expr {
    NumberExpr(double v) { value = v; }
    double value;
};

struct VariableExpr : Expr {
    VariableExpr(std::string n) { name = n; }
    std::string name;
};

struct BinaryExpr : Expr {
    BinaryExpr(BinaryOp op, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs) {
        operation = op;
        left = std::move(lhs);
        right = std::move(rhs);
    }
    BinaryOp operation;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
};

struct Stmt {
    virtual ~Stmt() = default;
};

struct LetStmt : Stmt {
    LetStmt(std::string n, std::unique_ptr<Expr> v) {
        name = n;
        value = std::move(v);
    }
    std::string name;
    std::unique_ptr<Expr> value;
};

struct PrintStmt : Stmt {
    PrintStmt(std::unique_ptr<Expr> v) {
        value = std::move(v);
    }
    std::unique_ptr<Expr> value;
};

using StmtPtr = std::unique_ptr<Stmt>;
using ExprPtr = std::unique_ptr<Expr>;
using Program = std::vector<StmtPtr>;
