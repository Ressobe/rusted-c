#ifndef AST_H
#define AST_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

enum class NodeType {
  // Statements
  Program,
  VarDeclaration,
  FunctionDeclaration,
  StructDeclaration,
  IfStatement,
  WhileLoop,
  ReturnStatement,

  // Expressions
  AssignmentExpr,
  NumericLiteral,
  StrLiteral,
  Null,
  Identifier,
  BinaryExpr,
  CallExpr,
  MemberAccessExpr,
  UnaryExpr,
  LogicalExpr,
};

class Node {
public:
  NodeType kind;
};

class Stmt : public Node {
public:
  Stmt(NodeType kind);
  virtual ~Stmt() = default;
};

class Expr : public Stmt {
public:
  Expr(NodeType kind);
  virtual ~Expr() = default;
};

class Program : public Stmt {
public:
  std::vector<std::unique_ptr<Stmt>> body;
  Program();
};

class AssignmentExpr : public Expr {
public:
  std::unique_ptr<Expr> assigne;
  std::unique_ptr<Expr> value;
  AssignmentExpr(std::unique_ptr<Expr> assigne, std::unique_ptr<Expr> value);
};

class VarDeclaration : public Stmt {
public:
  bool constant;
  std::string identifier;
  std::unique_ptr<Expr> value;
  VarDeclaration(bool isConst, const std::string &id,
                 std::unique_ptr<Expr> val = nullptr);
};

class ReturnStatement : public Stmt {
public:
  std::unique_ptr<Stmt> returnValue;
  ReturnStatement(std::unique_ptr<Stmt> value);
};

class FunctionDeclaration : public Stmt {
public:
  std::vector<std::string> parameters;
  std::string name;
  std::vector<Stmt *> body;
  std::unique_ptr<ReturnStatement> returnStatement;
  FunctionDeclaration(std::vector<std::string> param, std::string n,
                      std::vector<Stmt *> b,
                      std::unique_ptr<ReturnStatement> retStmt = nullptr);
  ~FunctionDeclaration();
};

class BinaryExpr : public Expr {
public:
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;
  std::string binaryOperator;
  BinaryExpr(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right,
             const std::string &op);
};

class UnaryExpr : public Expr {
public:
  std::unique_ptr<Expr> right;
  std::string op;
  UnaryExpr(std::unique_ptr<Expr> right, const std::string &op);
};

class CallExpr : public Expr {
public:
  std::unique_ptr<Expr> caller;
  std::vector<std::unique_ptr<Expr>> args;
  CallExpr(std::unique_ptr<Expr> caller,
           std::vector<std::unique_ptr<Expr>> args);
};

class IdentifierExpr : public Expr {
public:
  std::string symbol;
  IdentifierExpr(const std::string &symbol);
};

class NumericLiteral : public Expr {
public:
  double value;
  NumericLiteral(double value);
};

class StrLiteral : public Expr {
public:
  std::string value;
  StrLiteral(std::string value);
};

class FloatLiteral : public Expr {
public:
  double value;
  FloatLiteral(double value);
};

class NullLiteral : public Expr {
public:
  std::string value;
  NullLiteral(const std::string &value);
};

class IfStatement : public Stmt {
public:
  std::unique_ptr<Expr> condition;
  std::vector<std::unique_ptr<Stmt>> ifBody;
  std::vector<std::unique_ptr<Stmt>> elseBody;
  IfStatement(std::unique_ptr<Expr> cond,
              std::vector<std::unique_ptr<Stmt>> ifB,
              std::vector<std::unique_ptr<Stmt>> elseB = {});
};

class WhileLoop : public Stmt {
public:
  std::unique_ptr<Expr> condition;
  std::vector<std::unique_ptr<Stmt>> loopBody;
  WhileLoop(std::unique_ptr<Expr> cond, std::vector<std::unique_ptr<Stmt>> bd);
};

class StructDeclaration : public Stmt {
public:
  std::string structName;
  std::vector<std::unique_ptr<Stmt>> structBody;
  StructDeclaration(const std::string &name,
                    std::vector<std::unique_ptr<Stmt>> body);
};

class MemberAccessExpr : public Expr {
public:
  std::unique_ptr<Expr> object;
  std::string memberName;
  MemberAccessExpr(std::unique_ptr<Expr> obj, const std::string &member);
};

class LogicalExpr : public Expr {
public:
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    std::string logicalOperator;

    LogicalExpr(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right, const std::string& logicalOperator);
};

std::string NodeTypeToString(NodeType type);

void printProgram(std::unique_ptr<Program> program, const std::string &indent);

void printStatement(const Stmt &stmt, const std::string &indent);

#endif
