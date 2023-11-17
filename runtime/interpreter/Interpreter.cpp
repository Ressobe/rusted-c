#include "Interpreter.h"


RuntimeVal* Interpreter::evaluate(std::unique_ptr<Stmt> astNode, Environment* env) {
    std::cout << "fuck me" << std::endl;
    if (astNode->kind == NodeType::NumericLiteral) {
        NumericLiteral* numLiteral = dynamic_cast<NumericLiteral*>(astNode.get());
        return new NumberVal(numLiteral->value);
    }
    else if (astNode->kind == NodeType::Null) {
        return new NullVal();
    }
    else if (astNode->kind == NodeType::Identifier) {
        auto ident = dynamic_cast<IdentifierExpr*>(astNode.get());
        return eval_identifer(ident, env);
    }
    else if (astNode->kind == NodeType::BinaryExpr) {
        auto binary = dynamic_cast<BinaryExpr*>(astNode.get());
        return eval_binary_expr(binary, env);
    }
    // else if (astNode->kind == NodeType::UnaryExpr) {
    //     return eval_unary_expr(std::unique_ptr<UnaryExpr>(dynamic_cast<UnaryExpr*>(astNode.release())), env);
    // }
    // else if (astNode->kind == NodeType::Program) {
    //     return eval_program(std::unique_ptr<Program>(dynamic_cast<Program*>(astNode.release())), env);
    // }
    // else if (astNode->kind == NodeType::VarDeclaration) {
    //     return eval_var_declaration(std::unique_ptr<VarDeclaration>(dynamic_cast<VarDeclaration*>(astNode.release())), env);
    // }
    // else if (astNode->kind == NodeType::AssignmentExpr) {
    //     return eval_assignment(std::unique_ptr<AssignmentExpr>(dynamic_cast<AssignmentExpr*>(astNode.release())), env);
    // }
    // else if (astNode->kind == NodeType::CallExpr) {
    //     return eval_call_expr(std::unique_ptr<CallExpr>(dynamic_cast<CallExpr*>(astNode.release())), env);
    // }
    // else if (astNode->kind == NodeType::FunctionDeclaration) {
    //     return eval_function_declaration(std::unique_ptr<FunctionDeclaration>(dynamic_cast<FunctionDeclaration*>(astNode.release())), env);
    // }
    // else if (astNode->kind == NodeType::IfStatement) {
    //     return eval_if_statement(std::unique_ptr<IfStatement>(dynamic_cast<IfStatement*>(astNode.release())), env);
    // }
    // else if (astNode->kind == NodeType::WhileLoop) {
    //     return eval_while_statement(std::unique_ptr<WhileLoop>(dynamic_cast<WhileLoop*>(astNode.release())), env);
    // }
    // else if (astNode->kind == NodeType::ReturnStatement) {
    //     return eval_return_statement(std::unique_ptr<ReturnStatement>(dynamic_cast<ReturnStatement*>(astNode.release())), env);
    // }
    else {
        std::cerr << "This AST Node has not yet been set up for interpretation." << std::endl;
        std::exit(1);
    }
}
