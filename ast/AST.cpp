#include "AST.h"

Stmt::Stmt(NodeType kind) {
    this->kind = kind;
}

Expr::Expr(NodeType kind) : Stmt(kind) {}

Program::Program() : Stmt(NodeType::Program) {}

VarDeclaration::VarDeclaration(bool isConst, const std::string& id, Expr* val)
    : Stmt(NodeType::VarDeclaration), constant(isConst), identifier(id), value(val) {}

BinaryExpr::BinaryExpr(Expr* left, Expr* right, const std::string& op)
    : Expr(NodeType::BinaryExpr), left(left), right(right), binaryOperator(op) {}

IdentifierExpr::IdentifierExpr(const std::string& symbol) : Expr(NodeType::Identifier), symbol(symbol) {}

NumericLiteral::NumericLiteral(double value) : Expr(NodeType::NumericLiteral), value(value) {}

NullLiteral::NullLiteral(const std::string& value) : Expr(NodeType::Null), value(value) {}

AssignmentExpr::AssignmentExpr(Expr* assigne, Expr* val)
    : Expr(NodeType::AssignmentExpr), assigne(assigne), value(val) {}

CallExpr::CallExpr(Expr* caller, std::vector<Expr*> args) : Expr(NodeType::CallExpr), caller(caller), args(args) {}

FunctionDeclaration::FunctionDeclaration(std::vector<std::string> param, std::string n, std::vector<Stmt*> b) 
    : Stmt(NodeType::FunctionDeclaration), parameters(param), name(n), body(b) {}

IfStatement::IfStatement(Expr* cond, Stmt* ifB, Stmt* elseB)
    : Stmt(NodeType::IfStatement), condition(cond), ifBody(ifB), elseBody(elseB) {}

void printProgram(const Program& program, const std::string& indent) {
    std::cout << std::endl;
    std::cout << indent << " \"Program\": {\n";
    for (const Stmt* stmt : program.body) {
        printStatement(*stmt, indent + "  ");
        if (stmt != program.body.back()) {
            std::cout << ",";
        }
        std::cout << "\n";
    }
    std::cout << indent << "}";
    std::cout << "\n}\n";
}


void printStatement(const Stmt& stmt, const std::string& indent) {
    std::cout << indent << "{\n";
    std::cout << indent << "  \"Statement\": \"" << NodeTypeToString(stmt.kind) << "\",\n";

    if (stmt.kind == NodeType::Identifier) {
        const IdentifierExpr& id = static_cast<const IdentifierExpr&>(stmt);
        std::cout << indent << "  \"Symbol\": \"" << id.symbol << "\"";
    }
    else if (stmt.kind == NodeType::NumericLiteral) {
        const NumericLiteral& numLit = static_cast<const NumericLiteral&>(stmt);
        std::cout << indent << "  \"Value\": " << numLit.value;
    }
    else if (stmt.kind == NodeType::BinaryExpr) {
        const BinaryExpr& binaryExpr = static_cast<const BinaryExpr&>(stmt);
        std::cout << indent << "  \"BinaryOperator\": \"" << binaryExpr.binaryOperator << "\",\n";
        std::cout << indent << "  \"Left\": ";
        printStatement(*binaryExpr.left, indent + "    ");
        std::cout << ",\n";
        std::cout << indent << "  \"Right\": ";
        printStatement(*binaryExpr.right, indent + "    ");
    }
    else if (stmt.kind == NodeType::VarDeclaration) {
        const VarDeclaration& varDecl = static_cast<const VarDeclaration&>(stmt);
        std::cout << indent << "  \"Constant\": " << (varDecl.constant ? "true" : "false") << ",\n";
        std::cout << indent << "  \"Identifier\": \"" << varDecl.identifier << "\",\n";
        std::cout << indent << "  \"Value\": ";
        if (varDecl.value) {
            printStatement(*varDecl.value, indent + "    ");
        }
        else {
            std::cout << "null";
        }
    }
    else if (stmt.kind == NodeType::CallExpr) {
        const CallExpr& callExpr = static_cast<const CallExpr&>(stmt);
        std::cout << indent << "  \"Caller\": ";
        printStatement(*callExpr.caller, indent + "    ");
        std::cout << ",\n";
        std::cout << indent << "  \"Arguments\": [\n";
        for (const Expr* arg : callExpr.args) {
            printStatement(*arg, indent + "    ");
            if (arg != callExpr.args.back()) {
                std::cout << ",";
            }
            std::cout << "\n";
        }
        std::cout << indent << "  ]";
    }
    else if (stmt.kind == NodeType::FunctionDeclaration) {
        const FunctionDeclaration& funcDecl = static_cast<const FunctionDeclaration&>(stmt);
        std::cout << indent << "  \"Name\": \"" << funcDecl.name << "\",\n";
        std::cout << indent << "  \"Parameters\": [\n";
        for (const std::string& param : funcDecl.parameters) {
            std::cout << indent << "    \"" << param << "\"";
            if (&param != &funcDecl.parameters.back()) {
                std::cout << ",";
            }
            std::cout << "\n";
        }
        std::cout << indent << "  ],\n";
        std::cout << indent << "  \"Body\": [\n";
        for (const Stmt* bodyStmt : funcDecl.body) {
            printStatement(*bodyStmt, indent + "    ");
            if (bodyStmt != funcDecl.body.back()) {
                std::cout << ",";
            }
            std::cout << "\n";
        }
        std::cout << indent << "  ]";
    }
    else if (stmt.kind == NodeType::IfStatement) {
        const IfStatement& ifStmt = static_cast<const IfStatement&>(stmt);
        std::cout << indent << "  \"Condition\": ";
        printStatement(*ifStmt.condition, indent + "    ");
        std::cout << ",\n";
        std::cout << indent << "  \"IfBody\": ";
        printStatement(*ifStmt.ifBody, indent + "    ");
        if (ifStmt.elseBody) {
            std::cout << ",\n";
            std::cout << indent << "  \"ElseBody\": ";
            printStatement(*ifStmt.elseBody, indent + "    ");
        }
    }

    std::cout << "\n" << indent << "}";
}

std::string NodeTypeToString(NodeType type) {
	std::string value;
	switch (type) {
		case NodeType::Program:
			value = "Program";
			break;
		case NodeType::NumericLiteral:
			value = "NumericLiteral";
			break;
		case NodeType::Identifier:
			value =  "Identifier";
			break;
		case NodeType::BinaryExpr:
			value =  "BinaryExpr";
			break;
    case NodeType::VarDeclaration:
        value = "VarDeclaration";
        break;
    case NodeType::CallExpr:
        value = "CallExpr";
        break;
    case NodeType::FunctionDeclaration:
        value = "FunctionDeclaration";
        break;
    case NodeType::IfStatement:
        value = "IfStatement";
        break;
    case NodeType::Null:
        value = "Null";
        break;
		default:
			value = "Unknown"; // Handle unknown enum values
			break;
	}
	return value;
}
