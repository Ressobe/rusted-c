#include "AST.h"

void printProgram(std::unique_ptr<Program> program, const std::string &indent) {
  std::cout << '{' << std::endl;
  std::cout << indent << " \"Program\": {\n";
  for (const auto &stmt : program->body) {
    printStatement(*stmt, indent + "  ");
    if (stmt != program->body.back()) {
      std::cout << ',';
    }
    std::cout << '\n';
  }
  std::cout << indent << '}';
  std::cout << "\n}\n";
}

void printStatement(const Stmt &stmt, const std::string &indent) {
  std::cout << indent << "{\n";
  std::cout << indent << "  \"Statement\": \"" << NodeTypeToString(stmt.kind)
            << "\",\n";

  switch (stmt.kind) {
  case NodeType::Identifier: {
    const auto &id = static_cast<const IdentifierExpr &>(stmt);
    std::cout << indent << "  \"Symbol\": \"" << id.symbol << "\"";
    break;
  }
  case NodeType::NumericLiteral: {
    const auto &numLit = static_cast<const NumericLiteral &>(stmt);
    std::cout << indent << "  \"Value\": " << numLit.value;
    break;
  }
  case NodeType::StrLiteral: {
    const auto &strLit = static_cast<const StrLiteral &>(stmt);
    std::cout << indent << "  \"Value\": \"" << strLit.value << "\"";
    break;
  }
  case NodeType::BinaryExpr: {
    const auto &binaryExpr = static_cast<const BinaryExpr &>(stmt);
    std::cout << indent << "  \"BinaryOperator\": \""
              << binaryExpr.binaryOperator << "\",\n";
    std::cout << indent << "  \"Left\": ";
    printStatement(*binaryExpr.left, indent + "    ");
    std::cout << ",\n";
    std::cout << indent << "  \"Right\": ";
    printStatement(*binaryExpr.right, indent + "    ");
    break;
  }
  case NodeType::VarDeclaration: {
    const auto &varDecl = static_cast<const VarDeclaration &>(stmt);
    std::cout << indent
              << "  \"Constant\": " << (varDecl.constant ? "true" : "false")
              << ",\n";
    std::cout << indent << "  \"Identifier\": \"" << varDecl.identifier
              << "\",\n";
    std::cout << indent << "  \"Value\": ";
    if (varDecl.value) {
      printStatement(*varDecl.value, indent + "    ");
    } else {
      std::cout << "null";
    }
    break;
  }
  case NodeType::CallExpr: {
    const auto &callExpr = static_cast<const CallExpr &>(stmt);
    std::cout << indent << "  \"Caller\": ";
    printStatement(*callExpr.caller, indent + "    ");
    std::cout << ",\n";
    std::cout << indent << "  \"Arguments\": [\n";
    for (const auto &arg : callExpr.args) {
      printStatement(*arg, indent + "    ");
      if (&arg != &callExpr.args.back()) {
        std::cout << ",";
      }
      std::cout << "\n";
    }
    std::cout << indent << "  ]";
    break;
  }
  case NodeType::FunctionDeclaration: {
    const auto &funcDecl = static_cast<const FunctionDeclaration &>(stmt);
    std::cout << indent << "  \"Name\": \"" << funcDecl.name << "\",\n";
    std::cout << indent << "  \"Parameters\": [\n";
    for (const auto &param : funcDecl.parameters) {
      std::cout << indent << "    \"" << param << "\"";
      if (&param != &funcDecl.parameters.back()) {
        std::cout << ",";
      }
      std::cout << "\n";
    }
    std::cout << indent << "  ],\n";
    std::cout << indent << "  \"Body\": [\n";
    for (const auto &bodyStmt : funcDecl.body) {
      printStatement(*bodyStmt, indent + "    ");
      if (&bodyStmt != &funcDecl.body.back()) {
        std::cout << ",";
      }
      std::cout << "\n";
    }
    std::cout << indent << "  ]";
    break;
  }
  case NodeType::IfStatement: {
    const auto &ifStmt = static_cast<const IfStatement &>(stmt);
    std::cout << indent << "  \"Condition\": ";
    printStatement(*ifStmt.condition, indent + "    ");
    std::cout << ",\n";
    std::cout << indent << "  \"IfBody\": [\n";
    for (const auto &ifBodyStmt : ifStmt.ifBody) {
      printStatement(*ifBodyStmt, indent + "    ");
      if (&ifBodyStmt != &ifStmt.ifBody.back()) {
        std::cout << ",";
      }
      std::cout << "\n";
    }
    std::cout << indent << "  ],\n";
    std::cout << indent << "  \"ElseBody\": [\n";
    for (const auto &elseBodyStmt : ifStmt.elseBody) {
      printStatement(*elseBodyStmt, indent + "    ");
      if (&elseBodyStmt != &ifStmt.elseBody.back()) {
        std::cout << ",";
      }
      std::cout << "\n";
    }
    std::cout << indent << "  ]";
    break;
  }
  case NodeType::WhileLoop: {
    const auto &whileLoop = static_cast<const WhileLoop &>(stmt);
    std::cout << indent << "  \"Condition\": ";
    printStatement(*whileLoop.condition, indent + "    ");
    std::cout << ",\n";
    std::cout << indent << "  \"LoopBody\": [\n";
    for (const auto &loopBodyStmt : whileLoop.loopBody) {
      printStatement(*loopBodyStmt, indent + "    ");
      if (&loopBodyStmt != &whileLoop.loopBody.back()) {
        std::cout << ",";
      }
      std::cout << "\n";
    }
    std::cout << indent << "  ]";
    break;
  }
  case NodeType::StructDeclaration: {
    const StructDeclaration &structDecl =
        static_cast<const StructDeclaration &>(stmt);
    std::cout << indent << "  \"StructName\": \"" << structDecl.structName
              << "\"";
    for (const auto &stmt : structDecl.structBody) {
      printStatement(*stmt, indent + "  ");
    }
    break;
  }
  case NodeType::MemberAccessExpr: {
    const auto &memberAccessExpr = static_cast<const MemberAccessExpr &>(stmt);
    std::cout << indent << "  \"Object\": ";
    printStatement(*memberAccessExpr.object, indent + "    ");
    std::cout << ",\n";
    std::cout << indent << "  \"MemberName\": \"" << memberAccessExpr.memberName
              << "\"";
    break;
  }
  case NodeType::LogicalExpr: {
	const auto &logicalExpr = static_cast<const LogicalExpr &>(stmt);
	std::cout << indent << "  \"LogicalOperator\": \"" << logicalExpr.logicalOperator << "\",\n";
	std::cout << indent << "  \"Left\": ";
	printStatement(*logicalExpr.left, indent + "    ");
	std::cout << ",\n";
	std::cout << indent << "  \"Right\": ";
	printStatement(*logicalExpr.right, indent + "    ");
	break;
  }
  case NodeType::ReturnStatement: {
    const auto &returnStmt = static_cast<const ReturnStatement &>(stmt);
    std::cout << indent << "  \"ReturnValue\": ";
    printStatement(*returnStmt.returnValue, indent + "    ");
    break;
  }
  case NodeType::AssignmentExpr: {
    const auto &assignmentExpr = static_cast<const AssignmentExpr &>(stmt);
    std::cout << indent << "  \"Assignee\": ";
    printStatement(*assignmentExpr.assigne, indent + "    ");
    std::cout << ",\n";
    std::cout << indent << "  \"Value\": ";
    printStatement(*assignmentExpr.value, indent + "    ");
    break;
  }
  case NodeType::Program: {
    const auto &program = static_cast<const Program &>(stmt);
    std::cout << indent << "  \"Body\": [\n";
    for (const auto &bodyStmt : program.body) {
      printStatement(*bodyStmt, indent + "    ");
      if (&bodyStmt != &program.body.back()) {
        std::cout << ",";
      }
      std::cout << "\n";
    }
    std::cout << indent << "  ]";
    break;
  }
  case NodeType::Null: {
    const auto &nullNode = static_cast<const NullLiteral &>(stmt);
    std::cout << indent << "  \"Value\": \"" << nullNode.value << "\"";
    break;
  }
  case NodeType::UnaryExpr: {
    const auto &unaryExpr = static_cast<const UnaryExpr &>(stmt);
    std::cout << indent << "  \"Operator\": \"" << unaryExpr.op << "\",\n";
    std::cout << indent << "  \"Right\": ";
    printStatement(*unaryExpr.right, indent + "    ");
    break;
  }
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
  case NodeType::AssignmentExpr:
    return "AssigmentExpr";
  case NodeType::VarDeclaration:
    return "VarDeclaration";
  case NodeType::CallExpr:
    return "CallExpr";
  case NodeType::MemberAccessExpr:
    return "MemberAccessExpr";
  case NodeType::LogicalExpr:
    return "LogicalExpr";
  case NodeType::FunctionDeclaration:
    return "FunctionDeclaration";
  case NodeType::IfStatement:
    return "IfStatement";
  case NodeType::WhileLoop:
    return "WhileLoop";
  case NodeType::StructDeclaration:
    return "StructDeclaration";
  case NodeType::ReturnStatement:
    return "ReturnStatement";
  case NodeType::Null:
    return "Null";
  default:
    return "Unknown";
  }
}
