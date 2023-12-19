#include "Interpreter.h"

RuntimeVal *Interpreter::evaluate(Stmt *astNode, Environment *env) {
  RuntimeVal *result = nullptr;

  switch (astNode->kind) {
  case NodeType::NumericLiteral: {
    result = new NumberVal(dynamic_cast<NumericLiteral *>(astNode));
    break;
  }
  case NodeType::StrLiteral: {
    result = new StringVal(dynamic_cast<StrLiteral *>(astNode));
    break;
  }
  case NodeType::Null: {
    result = new NullVal();
    break;
  }
  case NodeType::Identifier: {
    result = Interpreter::eval_identifer(
        dynamic_cast<IdentifierExpr *>(astNode), env);
    break;
  }
  case NodeType::LogicalExpr: {
    result =
        Interpreter::eval_logical_expr(dynamic_cast<LogicalExpr *>(astNode), env);
    break;
  }
  case NodeType::BinaryExpr: {
    result =
        Interpreter::eval_binary_expr(dynamic_cast<BinaryExpr *>(astNode), env);
    break;
  }
  case NodeType::UnaryExpr: {
    result =
        Interpreter::eval_unary_expr(dynamic_cast<UnaryExpr *>(astNode), env);
    break;
  }
  case NodeType::Program: {
    result = Interpreter::eval_program(dynamic_cast<Program *>(astNode), env);
    break;
  }
  case NodeType::VarDeclaration: {
    result = Interpreter::eval_var_declaration(
        dynamic_cast<VarDeclaration *>(astNode), env);
    break;
  }
  case NodeType::AssignmentExpr: {
    result = Interpreter::eval_assignment(
        dynamic_cast<AssignmentExpr *>(astNode), env);
    break;
  }
  case NodeType::CallExpr: {
    result =
        Interpreter::eval_call_expr(dynamic_cast<CallExpr *>(astNode), env);
    break;
  }
  case NodeType::MemberAccessExpr: {
    result = Interpreter::eval_member_access(
        dynamic_cast<MemberAccessExpr *>(astNode), env);
    break;
  }
  case NodeType::FunctionDeclaration: {
    result = Interpreter::eval_function_declaration(
        dynamic_cast<FunctionDeclaration *>(astNode), env);
    break;
  }
  case NodeType::StructDeclaration: {
    result = Interpreter::eval_struct_declaration(
        dynamic_cast<StructDeclaration *>(astNode), env);
    break;
  }
  case NodeType::IfStatement: {
    result = Interpreter::eval_if_statement(
        dynamic_cast<IfStatement *>(astNode), env);
    break;
  }
  case NodeType::WhileLoop: {
    result = Interpreter::eval_while_statement(
        dynamic_cast<WhileLoop *>(astNode), env);
    break;
  }
  case NodeType::ReturnStatement: {
    result = Interpreter::eval_return_statement(
        dynamic_cast<ReturnStatement *>(astNode), env);
    break;
  }
  default: {
    std::cerr << "This AST Node has not yet been set up for interpretation."
              << std::endl;
    std::exit(1);
    break;
  }
  }

  return result;
}
