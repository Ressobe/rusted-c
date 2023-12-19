#include "AST.h"

Stmt::Stmt(NodeType kind) { this->kind = kind; }
Expr::Expr(NodeType kind) : Stmt(kind) {}

Program::Program() : Stmt(NodeType::Program) {}

VarDeclaration::VarDeclaration(bool isConst, const std::string &id,
                               std::unique_ptr<Expr> val)
    : Stmt(NodeType::VarDeclaration), constant(isConst), identifier(id),
      value(std::move(val)) {}

BinaryExpr::BinaryExpr(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right,
                       const std::string &op)
    : Expr(NodeType::BinaryExpr), left(std::move(left)),
      right(std::move(right)), binaryOperator(op) {}

UnaryExpr::UnaryExpr(std::unique_ptr<Expr> right, const std::string &op)
    : Expr(NodeType::UnaryExpr), right(std::move(right)), op(op) {}

IdentifierExpr::IdentifierExpr(const std::string &symbol)
    : Expr(NodeType::Identifier), symbol(symbol) {}

NumericLiteral::NumericLiteral(double value)
    : Expr(NodeType::NumericLiteral), value(value) {}

FloatLiteral::FloatLiteral(double value)
    : Expr(NodeType::NumericLiteral), value(value) {}

StrLiteral::StrLiteral(std::string value)
    : Expr(NodeType::StrLiteral), value(value) {}

NullLiteral::NullLiteral(const std::string &value)
    : Expr(NodeType::Null), value(value) {}

AssignmentExpr::AssignmentExpr(std::unique_ptr<Expr> assigne,
                               std::unique_ptr<Expr> val)
    : Expr(NodeType::AssignmentExpr), assigne(std::move(assigne)),
      value(std::move(val)) {}

CallExpr::CallExpr(std::unique_ptr<Expr> caller,
                   std::vector<std::unique_ptr<Expr>> args)
    : Expr(NodeType::CallExpr), caller(std::move(caller)),
      args(std::move(args)) {}

MemberAccessExpr::MemberAccessExpr(std::unique_ptr<Expr> obj,
                                   const std::string &member)
    : Expr(NodeType::MemberAccessExpr), object(std::move(obj)),
      memberName(member) {}

FunctionDeclaration::FunctionDeclaration(
    std::vector<std::string> param, std::string n, std::vector<Stmt *> b,
    std::unique_ptr<ReturnStatement> retStmt)
    : Stmt(NodeType::FunctionDeclaration), parameters(param), name(n), body(b),
      returnStatement(std::move(retStmt)) {}

FunctionDeclaration::~FunctionDeclaration() {
  for (auto &stmt : body) {
    delete stmt;
  }
}

IfStatement::IfStatement(std::unique_ptr<Expr> cond,
                         std::vector<std::unique_ptr<Stmt>> ifB,
                         std::vector<std::unique_ptr<Stmt>> elseB)
    : Stmt(NodeType::IfStatement), condition(std::move(cond)),
      ifBody(std::move(ifB)), elseBody(std::move(elseB)) {}

WhileLoop::WhileLoop(std::unique_ptr<Expr> cond,
                     std::vector<std::unique_ptr<Stmt>> bd)
    : Stmt(NodeType::WhileLoop), condition(std::move(cond)),
      loopBody(std::move(bd)) {}

ReturnStatement::ReturnStatement(std::unique_ptr<Stmt> value)
    : Stmt(NodeType::ReturnStatement), returnValue(std::move(value)) {}

StructDeclaration::StructDeclaration(const std::string &name,
                                     std::vector<std::unique_ptr<Stmt>> body)
    : Stmt(NodeType::StructDeclaration), structName(name),
      structBody(std::move(body)) {}

LogicalExpr::LogicalExpr(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right, const std::string& logicalOperator)
        : Expr(NodeType::LogicalExpr), left(std::move(left)), right(std::move(right)), logicalOperator(logicalOperator) {}
