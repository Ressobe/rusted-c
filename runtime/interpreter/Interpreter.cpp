#include "Interpreter.h"

RuntimeVal* Interpreter::evaluate(Stmt* astNode, Environment* env) {
    if (astNode->kind == NodeType::NumericLiteral) {
        NumericLiteral* numLiteral = dynamic_cast<NumericLiteral*>(astNode);
        return NumberVal::MK_NUMBER(numLiteral->value);
    }
    else if (astNode->kind == NodeType::Null) {
        return NullVal::MK_NULL();
    }
    else if (astNode->kind == NodeType::Identifier) {
        return EvaluateExpression::eval_identifer(dynamic_cast<IdentifierExpr*>(astNode), env);
    }
    else if (astNode->kind == NodeType::BinaryExpr) {
        return EvaluateExpression::eval_binary_expr(dynamic_cast<BinaryExpr*>(astNode), env);
    }
    else if (astNode->kind == NodeType::Program) {
        return EvaluateStatement::eval_program(dynamic_cast<Program*>(astNode), env);
    }
    else if (astNode->kind == NodeType::VarDeclaration) {
      return  EvaluateStatement::eval_var_declaration(dynamic_cast<VarDeclaration*>(astNode), env);
    }
    else if (astNode->kind == NodeType::AssignmentExpr) {
        return EvaluateExpression::eval_assignment(dynamic_cast<AssignmentExpr*>(astNode), env);
    }
    else if (astNode->kind == NodeType::CallExpr) {
        return EvaluateExpression::eval_call_expr(dynamic_cast<CallExpr*>(astNode), env);
    }
    else if (astNode->kind == NodeType::FunctionDeclaration) {
        return EvaluateStatement::eval_function_declaration(dynamic_cast<FunctionDeclaration*>(astNode), env);
    }
    else if (astNode->kind == NodeType::IfStatement) {
        return EvaluateStatement::eval_if_statement(dynamic_cast<IfStatement*>(astNode), env);
    }
    else if (astNode->kind == NodeType::ReturnStatement) {
        return EvaluateStatement::eval_return_statement(dynamic_cast<ReturnStatement*>(astNode), env);
    }
    else {
        std::cerr << "This AST Node has not yet been set up for interpretation." << std::endl;
        std::exit(1);
    }
}
