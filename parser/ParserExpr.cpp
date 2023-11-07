#include "Parser.h"


Expr* Parser::parse_expr() {
    return parse_assignment_expr();
}


Expr* Parser::parse_comprasion_expr() {
    Expr* left = parse_additive_expr();

    if (at().getType() == TokenType::LessThan ||
        at().getType() == TokenType::LessEqual ||
        at().getType() == TokenType::GreaterThan ||
        at().getType() == TokenType::GreaterEqual ||
        at().getType() == TokenType::EqualEqual ||
        at().getType() == TokenType::NotEqual) {
        std::string comparisonOperator = eat().getValue();
        Expr* right = parse_additive_expr();
        left = new BinaryExpr(left, right, comparisonOperator);
    }

    return left;
}

Expr* Parser::parse_primary_expr() {
    TokenType tk = at().getType();
    Expr* value = nullptr;

    if (tk == TokenType::Not) {
        this->eat();
        value = new UnaryExpr(parse_primary_expr(), "!");
    } else {
        switch (tk) {
        case TokenType::Identifier:
                value = new IdentifierExpr(eat().getValue());
                break;
        case TokenType::NumberLiteral:
                value = new NumericLiteral(std::stod(eat().getValue()));
                break;
        case TokenType::Null:
                eat();
                value = new NullLiteral("null");
                break;
        case TokenType::OpenParen:
                eat();
                value = parse_expr();
                expect(TokenType::CloseParen, "Unexpected token found inside parenthesized expression. Expected closing parenthesis.");
                break;
        default:
          value = nullptr; // Initialize value as nullptr in the default case
          std::cerr << "Unexpected token found during parsing!" << at().getValue() << std::endl;
          std::exit(1);
        }
    }

    return value;
}

Expr* Parser::parse_additive_expr() {
    Expr* left = this->parse_multiplicative_expr();

    while (this->at().getValue() == "+" || this->at().getValue() == "-") {
        std::string binaryOperator = this->eat().getValue();
        Expr* right = this->parse_multiplicative_expr();
        left = new BinaryExpr(left, right, binaryOperator);
    }

    return left;
}


Expr* Parser::parse_multiplicative_expr() {
    Expr* left = this->parse_call_member_expr();

    while (at().getValue() == "/" || at().getValue() == "*" || at().getValue() == "%") {
        std::string binaryOperator = this->eat().getValue();
        Expr* right = this->parse_primary_expr();
        left = new BinaryExpr(left, right, binaryOperator);
    }

    return left;
}


Expr* Parser::parse_assignment_expr() {
    Expr* left = this->parse_additive_expr();

    if (this->at().getType() == TokenType::Equals) {
        this->eat();

        Expr* value = this->parse_assignment_expr();
        expect(TokenType::Semicolon, "Var declaration must end with a semicolon.");

        Expr* assignmentExpr = new AssignmentExpr(left, value);
        return assignmentExpr;
    }

    return left;
}


Expr* Parser::parse_call_member_expr() {
    Expr* caller = this->parse_primary_expr();

    while (this->at().getType() == TokenType::OpenParen) {
        this->eat();
        std::vector<Expr*> arguments;

        while (this->at().getType() != TokenType::CloseParen) {
            arguments.push_back(this->parse_expr());

            if (this->at().getType() == TokenType::Comma) {
                this->eat();
            }
        }

        expect(TokenType::CloseParen, "Expected a closing parenthesis in the function call.");

        if (this->at().getType() == TokenType::Semicolon) {
            this->eat();
        } else if (this->at().getType() != TokenType::Semicolon && this->at().getType() != TokenType::CloseParen) {
            std::cerr << "Parser Error:\nExpected a semicolon (;) at the end of the line or a closing parenthesis after function call." << std::endl;
            std::exit(1);
        }
        caller = new CallExpr(caller, arguments);
    }

    return caller;
}


Expr* Parser::parse_call_expr(Expr* caller) {
    Expr* call_expr = new CallExpr(caller,this->parse_args());

    while (this->at().getType() == TokenType::OpenParen) {
        call_expr = parse_call_expr(call_expr);
    }

    return call_expr;
}
