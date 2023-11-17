#include "Interpreter.h"


RuntimeVal* Interpreter::evaluate(Stmt* astNode, Environment* env) {
    if (astNode->kind == NodeType::NumericLiteral) {
        NumericLiteral* numLiteral = dynamic_cast<NumericLiteral*>(astNode);
        return new NumberVal(numLiteral->value);
    } else if (astNode->kind == NodeType::Null) {
        return new NullVal();
    } else if (astNode->kind == NodeType::StrLiteral) {
      StrLiteral* strLiteral = dynamic_cast<StrLiteral*>(astNode);
      return new StringVal(strLiteral->value);
    }
    else if (astNode->kind == NodeType::Identifier) {
        return Interpreter::eval_identifer(dynamic_cast<IdentifierExpr*>(astNode), env);
    }
    else if (astNode->kind == NodeType::BinaryExpr) {
        return Interpreter::eval_binary_expr(dynamic_cast<BinaryExpr*>(astNode), env);
    }
    else if (astNode->kind == NodeType::UnaryExpr) {
        return Interpreter::eval_unary_expr(dynamic_cast<UnaryExpr*>(astNode), env);
    }
    else if (astNode->kind == NodeType::Program) {
        return Interpreter::eval_program(dynamic_cast<Program*>(astNode), env);
    }
    else if (astNode->kind == NodeType::VarDeclaration) {
      return  Interpreter::eval_var_declaration(dynamic_cast<VarDeclaration*>(astNode), env);
    }
    else if (astNode->kind == NodeType::AssignmentExpr) {
        return Interpreter::eval_assignment(dynamic_cast<AssignmentExpr*>(astNode), env);
    }
    else if (astNode->kind == NodeType::CallExpr) {
        return Interpreter::eval_call_expr(dynamic_cast<CallExpr*>(astNode), env);
    }
    // else if (astNode->kind == NodeType::FunctionDeclaration) {
    //     return Interpreter::eval_function_declaration(dynamic_cast<FunctionDeclaration*>(astNode), env);
    // }
    else if (astNode->kind == NodeType::IfStatement) {
        return Interpreter::eval_if_statement(dynamic_cast<IfStatement*>(astNode), env);
    }
    else if (astNode->kind == NodeType::WhileLoop) {
        return Interpreter::eval_while_statement(dynamic_cast<WhileLoop*>(astNode), env);
    }
    else if (astNode->kind == NodeType::ReturnStatement) {
        return Interpreter::eval_return_statement(dynamic_cast<ReturnStatement*>(astNode), env);
    }
    else {
        std::cerr << "This AST Node has not yet been set up for interpretation." << std::endl;
        std::exit(1);
    }
}
