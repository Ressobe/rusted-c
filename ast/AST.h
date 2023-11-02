#pragma once

#ifndef AST_H
#define AST_H

#include <iostream>
#include <vector>
#include <string>

enum class NodeType {
	// Statements
  Program,
	VarDeclaration,
	FunctionDeclaration,    
  IfStatement,


	// Expressions
	AssignmentExpr,
  NumericLiteral,
	Null,
  Identifier,
  BinaryExpr,
	CallExpr,
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
		std::vector<Stmt*> body;
		Program();
};


class AssignmentExpr : public Expr {
	public:
		AssignmentExpr(Expr* assigne, Expr* value);
		Expr* assigne;
		Expr* value;
};


class VarDeclaration : public Stmt {
	public:
		bool constant;
		std::string identifier;
		Expr* value;
		VarDeclaration(bool isConst, const std::string& id, Expr* val = nullptr);
};

class FunctionDeclaration : public Stmt {
	public:
		std::vector<std::string> parameters;
		std::string name;
		std::vector<Stmt*> body;
		FunctionDeclaration(std::vector<std::string> param, std::string n, std::vector<Stmt*> b);
};


class BinaryExpr : public Expr {
	public:
		Expr* left;
		Expr* right;
		std::string binaryOperator;
		BinaryExpr(Expr* left, Expr* right, const std::string& op);
};

class CallExpr : public Expr {
	public:
		Expr* caller;
		std::vector<Expr*> args;
		CallExpr(Expr* caller, std::vector<Expr*> args);
};


class IdentifierExpr : public Expr {
	public:
		std::string symbol;
		IdentifierExpr(const std::string& symbol);
};

class NumericLiteral : public Expr {
	public:
		double value;
		NumericLiteral(double value);
};

class NullLiteral : public Expr {
	public:
		std::string value;
		NullLiteral(const std::string& value);
};

class IfStatement : public Stmt {
  public:
    Expr* condition;
    Stmt* ifBody;
    Stmt* elseBody;
    IfStatement(Expr* cond, Stmt* ifB, Stmt* elseB = nullptr);
};

// Convert enum numbers to string
std::string NodeTypeToString(NodeType type);

// Function to print the program and its body recursively
void printProgram(const Program& program, const std::string& indent);

// Function to print a statement and its body recursively
void printStatement(const Stmt& stmt, const std::string& indent);

#endif

