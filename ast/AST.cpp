#include "AST.h"

Stmt::Stmt(NodeType kind) {
    this->kind = kind;
}
Expr::Expr(NodeType kind) : Stmt(kind) {}

Program::Program() : Stmt(NodeType::Program) {}

VarDeclaration::VarDeclaration(bool isConst, const std::string& id, std::unique_ptr<Expr> val)
    : Stmt(NodeType::VarDeclaration), constant(isConst), identifier(id), value(std::move(val)) {}

BinaryExpr::BinaryExpr(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right, const std::string& op)
    : Expr(NodeType::BinaryExpr), left(std::move(left)), right(std::move(right)), binaryOperator(op) {}

UnaryExpr::UnaryExpr(std::unique_ptr<Expr> right, const std::string& op)
    : Expr(NodeType::UnaryExpr), right(std::move(right)), op(op) {}

IdentifierExpr::IdentifierExpr(const std::string& symbol)
    : Expr(NodeType::Identifier), symbol(symbol) {}

NumericLiteral::NumericLiteral(double value)
    : Expr(NodeType::NumericLiteral), value(value) {}

StrLiteral::StrLiteral(std::string value)
    : Expr(NodeType::StrLiteral), value(value) {}

NullLiteral::NullLiteral(const std::string& value)
    : Expr(NodeType::Null), value(value) {}

AssignmentExpr::AssignmentExpr(std::unique_ptr<Expr> assigne, std::unique_ptr<Expr> val)
    : Expr(NodeType::AssignmentExpr), assigne(std::move(assigne)), value(std::move(val)) {}

CallExpr::CallExpr(std::unique_ptr<Expr> caller, std::vector<std::unique_ptr<Expr>> args)
    : Expr(NodeType::CallExpr), caller(std::move(caller)), args(std::move(args)) {}

FunctionDeclaration::FunctionDeclaration(
    std::vector<std::string> param, std::string n, std::vector<std::unique_ptr<Stmt>> b, std::unique_ptr<ReturnStatement> retStmt)
    : Stmt(NodeType::FunctionDeclaration), parameters(param), name(n), body(std::move(b)), returnStatement(std::move(retStmt)) {}

IfStatement::IfStatement(std::unique_ptr<Expr> cond, std::vector<std::unique_ptr<Stmt>> ifB, std::vector<std::unique_ptr<Stmt>> elseB)
    : Stmt(NodeType::IfStatement), condition(std::move(cond)), ifBody(std::move(ifB)), elseBody(std::move(elseB)) {}

WhileLoop::WhileLoop(std::unique_ptr<Expr> cond, std::vector<std::unique_ptr<Stmt>> bd)
    : Stmt(NodeType::WhileLoop), condition(std::move(cond)), loopBody(std::move(bd)) {}

ReturnStatement::ReturnStatement(std::unique_ptr<Expr> value)
    : Stmt(NodeType::ReturnStatement), returnValue(std::move(value)) {}

void printProgram(const Program& program, const std::string& indent) {
    std::cout << std::endl;
    std::cout << indent << " \"Program\": {\n";
    for (const auto& stmt : program.body) {
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
        const auto& id = static_cast<const IdentifierExpr&>(stmt);
        std::cout << indent << "  \"Symbol\": \"" << id.symbol << "\"";
    } else if (stmt.kind == NodeType::NumericLiteral) {
        const auto& numLit = static_cast<const NumericLiteral&>(stmt);
        std::cout << indent << "  \"Value\": " << numLit.value;
    } else if (stmt.kind == NodeType::BinaryExpr) {
        const auto& binaryExpr = static_cast<const BinaryExpr&>(stmt);
        std::cout << indent << "  \"BinaryOperator\": \"" << binaryExpr.binaryOperator << "\",\n";
        std::cout << indent << "  \"Left\": ";
        printStatement(*binaryExpr.left, indent + "    ");
        std::cout << ",\n";
        std::cout << indent << "  \"Right\": ";
        printStatement(*binaryExpr.right, indent + "    ");
    } else if (stmt.kind == NodeType::VarDeclaration) {
        const auto& varDecl = static_cast<const VarDeclaration&>(stmt);
        std::cout << indent << "  \"Constant\": " << (varDecl.constant ? "true" : "false") << ",\n";
        std::cout << indent << "  \"Identifier\": \"" << varDecl.identifier << "\",\n";
        std::cout << indent << "  \"Value\": ";
        if (varDecl.value) {
            printStatement(*varDecl.value, indent + "    ");
        } else {
            std::cout << "null";
        }
    } else if (stmt.kind == NodeType::CallExpr) {
        const auto& callExpr = static_cast<const CallExpr&>(stmt);
        std::cout << indent << "  \"Caller\": ";
        printStatement(*callExpr.caller, indent + "    ");
        std::cout << ",\n";
        std::cout << indent << "  \"Arguments\": [\n";
        for (const auto& arg : callExpr.args) {
            printStatement(*arg, indent + "    ");
            if (&arg != &callExpr.args.back()) {
                std::cout << ",";
            }
            std::cout << "\n";
        }
        std::cout << indent << "  ]";
    } else if (stmt.kind == NodeType::FunctionDeclaration) {
        const auto& funcDecl = static_cast<const FunctionDeclaration&>(stmt);
        std::cout << indent << "  \"Name\": \"" << funcDecl.name << "\",\n";
        std::cout << indent << "  \"Parameters\": [\n";
        for (const auto& param : funcDecl.parameters) {
            std::cout << indent << "    \"" << param << "\"";
            if (&param != &funcDecl.parameters.back()) {
                std::cout << ",";
            }
            std::cout << "\n";
        }
        std::cout << indent << "  ],\n";
        std::cout << indent << "  \"Body\": [\n";
        for (const auto& bodyStmt : funcDecl.body) {
            printStatement(*bodyStmt, indent + "    ");
            if (&bodyStmt != &funcDecl.body.back()) {
                std::cout << ",";
            }
            std::cout << "\n";
        }
        std::cout << indent << "  ]";
    } else if (stmt.kind == NodeType::IfStatement) {
        const auto& ifStmt = static_cast<const IfStatement&>(stmt);
        std::cout << indent << "  \"Condition\": ";
        printStatement(*ifStmt.condition, indent + "    ");
        std::cout << ",\n";
        std::cout << indent << "  \"IfBody\": [\n";
        for (const auto& ifBodyStmt : ifStmt.ifBody) {
            printStatement(*ifBodyStmt, indent + "    ");
            if (&ifBodyStmt != &ifStmt.ifBody.back()) {
                std::cout << ",";
            }
            std::cout << "\n";
        }
        std::cout << indent << "  ],\n";
        std::cout << indent << "  \"ElseBody\": [\n";
        for (const auto& elseBodyStmt : ifStmt.elseBody) {
            printStatement(*elseBodyStmt, indent + "    ");
            if (&elseBodyStmt != &ifStmt.elseBody.back()) {
                std::cout << ",";
            }
            std::cout << "\n";
        }
        std::cout << indent << "  ]";
    } else if (stmt.kind == NodeType::WhileLoop) {
        const auto& whileLoop = static_cast<const WhileLoop&>(stmt);
        std::cout << indent << "  \"Condition\": ";
        printStatement(*whileLoop.condition, indent + "    ");
        std::cout << ",\n";
        std::cout << indent << "  \"LoopBody\": [\n";
        for (const auto& loopBodyStmt : whileLoop.loopBody) {
            printStatement(*loopBodyStmt, indent + "    ");
            if (&loopBodyStmt != &whileLoop.loopBody.back()) {
                std::cout << ",";
            }
            std::cout << "\n";
        }
        std::cout << indent << "  ]";
    } else if (stmt.kind == NodeType::ReturnStatement) {
        const auto& returnStmt = static_cast<const ReturnStatement&>(stmt);
        std::cout << indent << "  \"ReturnValue\": ";
        printStatement(*returnStmt.returnValue, indent + "    ");
    }

    std::cout << "\n" << indent << "}";
}

std::string NodeTypeToString(NodeType type) {
    switch (type) {
        case NodeType::Program:
            return "Program";
        case NodeType::NumericLiteral:
            return "NumericLiteral";
        case NodeType::StrLiteral:
            return "StrLiteral";
        case NodeType::Identifier:
            return "Identifier";
        case NodeType::BinaryExpr:
            return "BinaryExpr";
        case NodeType::VarDeclaration:
            return "VarDeclaration";
        case NodeType::CallExpr:
            return "CallExpr";
        case NodeType::FunctionDeclaration:
            return "FunctionDeclaration";
        case NodeType::IfStatement:
            return "IfStatement";
        case NodeType::WhileLoop:
            return "WhileLoop";
        case NodeType::ReturnStatement:
            return "ReturnStatement";
        case NodeType::Null:
            return "Null";
        default:
            return "Unknown";
    }
}
