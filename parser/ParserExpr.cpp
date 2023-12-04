#include "Parser.h"


using ExprPtr = std::unique_ptr<Expr>;
using StmtPtr = std::unique_ptr<Stmt>;


ExprPtr Parser::parse_expr() {
    return parse_assignment_expr();
}


ExprPtr Parser::parse_comprasion_expr() {
    ExprPtr left = parse_additive_expr();

    if (at().getType() == TokenType::LessThan ||
        at().getType() == TokenType::LessEqual ||
        at().getType() == TokenType::GreaterThan ||
        at().getType() == TokenType::GreaterEqual ||
        at().getType() == TokenType::EqualEqual ||
        at().getType() == TokenType::NotEqual) {
        std::string comparisonOperator = eat().getValue();

        ExprPtr right = parse_additive_expr();

        left = std::make_unique<BinaryExpr>(std::move(left), std::move(right), comparisonOperator);
    }

    return left;
}


ExprPtr Parser::parse_primary_expr() {
    TokenType tk = at().getType();
    ExprPtr value = nullptr;

    if (tk == TokenType::Not) {
        this->eat();
        value = std::make_unique<UnaryExpr>(parse_primary_expr(), "!");
    }
    else if (tk == TokenType::BinaryOperator && at().getValue() == "-" ) {
        this->eat();
        value = std::make_unique<UnaryExpr>(parse_primary_expr(), "-");
    }
    else {
        switch (tk) {
        case TokenType::Identifier:
            value = std::make_unique<IdentifierExpr>(eat().getValue());
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
            expect(TokenType::CloseParen, "Unexpected token found inside parenthesized expression. Expected closing parenthesis.");
            break;
        default:
            value = nullptr;
            std::cerr << "Unexpected token found during parsing!" << at().getValue() << std::endl;
            std::exit(1);
        }
    }

    return value;
}


ExprPtr Parser::parse_additive_expr() {
     ExprPtr left = this->parse_multiplicative_expr();

    while (this->at().getValue() == "+" || this->at().getValue() == "-") {
        std::string binaryOperator = this->eat().getValue();
        ExprPtr right = this->parse_multiplicative_expr();
        left = std::make_unique<BinaryExpr>(std::move(left), std::move(right), binaryOperator);
    }

    return left;
}


ExprPtr Parser::parse_multiplicative_expr() {
    ExprPtr left = this->parse_call_member_expr();

    while (at().getValue() == "/" || at().getValue() == "*" || at().getValue() == "%") {
        std::string binaryOperator = this->eat().getValue();
        ExprPtr right = this->parse_primary_expr();
        left = std::make_unique<BinaryExpr>(std::move(left), std::move(right), binaryOperator);
    }

    return left;
}


ExprPtr Parser::parse_assignment_expr() {
    ExprPtr left = this->parse_additive_expr();

    if (this->at().getType() == TokenType::Equals) {
        this->eat();

        ExprPtr value = this->parse_assignment_expr();
        
        this->expect(TokenType::Semicolon, "Expected semicolon at the end of assigment expression");
        
        return std::make_unique<AssignmentExpr>(std::move(left), std::move(value));
    }

    return left;
}

ExprPtr Parser::parse_call_member_expr() {
    ExprPtr caller = this->parse_primary_expr();

    while (this->at().getType() == TokenType::OpenParen) {
        this->eat();
        std::vector<ExprPtr> arguments;

        while (this->at().getType() != TokenType::CloseParen) {
            arguments.push_back(this->parse_expr());

            if (this->at().getType() == TokenType::Comma) {
                this->eat();
            }
        }

        this->expect(TokenType::CloseParen, "Expected a closing parenthesis in the function call");

        caller = std::make_unique<CallExpr>(std::move(caller), std::move(arguments));
    }
    return caller;
}


ExprPtr Parser::parse_call_expr(std::unique_ptr<Expr> caller) {
    ExprPtr call_expr = std::make_unique<CallExpr>(std::move(caller), this->parse_args());
    while (this->at().getType() == TokenType::OpenParen) {
        call_expr = parse_call_expr(std::move(call_expr));
    }
    return call_expr;
}


std::vector<ExprPtr> Parser::parse_args() {
    this->expect(TokenType::OpenParen, "Expected open parenthesis");
    std::vector<ExprPtr> args;

    if (this->at().getType() != TokenType::CloseParen) {
        args = this->parse_arguments_list();
    }
    this->expect(TokenType::CloseParen, "Missing closing parenthesis inside arguments list");

    return args;
}


std::vector<ExprPtr> Parser::parse_arguments_list() {
    std::vector<ExprPtr> args;
    args.push_back(this->parse_assignment_expr());

    while (this->at().getType() == TokenType::Comma) {
        this->eat();
        args.push_back(this->parse_assignment_expr());
    }

    return args;
}
