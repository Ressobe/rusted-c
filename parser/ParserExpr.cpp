#include "Parser.h"

using ExprPtr = std::unique_ptr<Expr>;
using StmtPtr = std::unique_ptr<Stmt>;

ExprPtr Parser::parse_expr() {
  try {
    return parse_assignment_expr();
  } catch (const ParserError &e) {
    throw;
  }
}

ExprPtr Parser::parse_logical_expr() {
  try {

    ExprPtr left = parse_comparision_expr();

    while (is_logical_operator(at().getType())) {
      std::string logicalOperator = eat().getValue();
      ExprPtr right = parse_comparision_expr();
      left = std::make_unique<LogicalExpr>(std::move(left), std::move(right),
                                           logicalOperator);
    }

    return left;
  } catch (const ParserError &e) {
    throw;
  }
}

ExprPtr Parser::parse_comparision_expr() {
  try {
    ExprPtr left = parse_additive_expr();

    if (is_comparison_operator(at().getType())) {
      std::string comparisonOperator = eat().getValue();
      ExprPtr right = parse_additive_expr();
      left = std::make_unique<BinaryExpr>(std::move(left), std::move(right),
                                          comparisonOperator);
    }

    return left;
  } catch (const ParserError &e) {
    throw;
  }
}

ExprPtr Parser::parse_primary_expr() {
  try {
    TokenType tk = at().getType();
    ExprPtr value = nullptr;

    if (tk == TokenType::Not) {
      this->eat();
      value = std::make_unique<UnaryExpr>(parse_primary_expr(), "!");
    } else if (tk == TokenType::BinaryOperator && at().getValue() == "-") {
      this->eat();
      value = std::make_unique<UnaryExpr>(parse_primary_expr(), "-");
    } else {
      switch (tk) {
      case TokenType::Identifier:
        value = parse_member_access(
            std::make_unique<IdentifierExpr>(eat().getValue()));
        break;
      case TokenType::NumberLiteral:
        value = std::make_unique<NumericLiteral>(std::stod(eat().getValue()));
        break;
      case TokenType::FloatLiteral:
        value = std::make_unique<NumericLiteral>(std::stod(eat().getValue()));
        break;
      case TokenType::StringLiteral:
        value = std::make_unique<StrLiteral>(eat().getValue());
        break;
      case TokenType::Null:
        eat();
        value = std::make_unique<NullLiteral>("null");
        break;
      case TokenType::OpenParen:
        eat();
        value = parse_expr();
        expect(
            TokenType::CloseParen,
            "Unexpected token found inside parenthesized expression. Expected "
            "closing parenthesis.");
        break;
      default:
        value = nullptr;
        std::cerr << "Unexpected token found during parsing! "
                  << at().getValue() << std::endl;
        std::exit(1);
      }
    }

    return value;

  } catch (const ParserError &e) {
    throw;
  }
}

ExprPtr Parser::parse_additive_expr() {
  try {
    ExprPtr left = parse_multiplicative_expr();

    while (is_additive_operator(at().getValue())) {
      std::string binaryOperator = eat().getValue();
      ExprPtr right = parse_multiplicative_expr();
      left = std::make_unique<BinaryExpr>(std::move(left), std::move(right),
                                          binaryOperator);
    }

    return left;
  } catch (const ParserError &e) {
    throw;
  }
}

ExprPtr Parser::parse_multiplicative_expr() {
  try {
    ExprPtr left = parse_call_member_expr();

    while (is_multiplicative_operator(at().getValue())) {
      std::string binaryOperator = eat().getValue();
      ExprPtr right = parse_primary_expr();
      left = std::make_unique<BinaryExpr>(std::move(left), std::move(right),
                                          binaryOperator);
    }

    return left;
  } catch (const ParserError &e) {
    throw;
  }
}

ExprPtr Parser::parse_assignment_expr() {
  try {
    ExprPtr left = parse_additive_expr();

    if (at().getType() == TokenType::Equals) {
      eat();
      ExprPtr value = parse_assignment_expr();
      expect(TokenType::Semicolon,
             "Expected semicolon at the end of assignment expression");
      return std::make_unique<AssignmentExpr>(std::move(left),
                                              std::move(value));
    }

    return left;
  } catch (const ParserError &e) {
    throw;
  }
}

ExprPtr Parser::parse_call_member_expr() {
  try {
    ExprPtr caller = parse_primary_expr();

    while (at().getType() == TokenType::OpenParen) {
      eat();
      std::vector<ExprPtr> arguments;

      if (at().getType() != TokenType::CloseParen) {
        arguments = parse_arguments_list();
      }

      expect(TokenType::CloseParen,
             "Expected a closing parenthesis in the function call");
      caller =
          std::make_unique<CallExpr>(std::move(caller), std::move(arguments));
    }

    return caller;
  } catch (const ParserError &e) {
    throw;
  }
}

ExprPtr Parser::parse_member_access(ExprPtr left) {
  try {
    while (at().getType() == TokenType::Dot ||
           at().getType() == TokenType::OpenParen) {
      if (at().getType() == TokenType::Dot) {
        eat(); // Consume the '.'
        std::string memberName =
            expect(TokenType::Identifier, "Expected identifier after '.'")
                .getValue();
        left = std::make_unique<MemberAccessExpr>(std::move(left),
                                                  std::move(memberName));
      } else if (at().getType() == TokenType::OpenParen) {
        std::vector<ExprPtr> arguments = parse_args();
        left =
            std::make_unique<CallExpr>(std::move(left), std::move(arguments));
      }
    }
    return left;
  } catch (const ParserError &e) {
    throw;
  }
}

std::vector<ExprPtr> Parser::parse_args() {
  try {
    expect(TokenType::OpenParen, "Expected open parenthesis");
    std::vector<ExprPtr> args;

    if (at().getType() != TokenType::CloseParen) {
      args = parse_arguments_list();
    }

    expect(TokenType::CloseParen,
           "Missing closing parenthesis inside arguments list");
    return args;
  } catch (const ParserError &e) {
    throw;
  }
}

std::vector<ExprPtr> Parser::parse_arguments_list() {
  try {
    std::vector<ExprPtr> args;
    args.push_back(parse_assignment_expr());

    while (at().getType() == TokenType::Comma) {
      eat();
      args.push_back(parse_assignment_expr());
    }

    return args;
  } catch (const ParserError &e) {
    throw;
  }
}
