#include "Parser.h"


bool Parser::eof() {
    return tokens[0].getType() == TokenType::EOFToken;
}

Token Parser::at() {
    return tokens[0];
}

Token Parser::eat() {
    Token prev = tokens[0];
    tokens.erase(tokens.begin());
    return prev;
}


Token Parser::lookahead(size_t num) {
    if (num >= tokens.size()) {
        return Token("", TokenType::EOFToken);
    }

    return tokens[num];
}


Token Parser::expect(TokenType type, const std::string& err) {
    Token prev = this->eat();
    if (prev.getType() != type) {
        std::cerr << "Parser Error:\n" << err << prev.getValue() << " - Expecting: " << static_cast<int>(type) << std::endl;
        std::exit(1);
    }
    return prev;
}


Program Parser::produceAST(std::vector<Token> tokens) {
    this->tokens = tokens;

    Program program;
    program.kind = NodeType::Program;

    while (!eof()) {
        program.body.push_back(parse_stmt());
    }

    return program;
}


std::vector<Expr*> Parser::parse_args() {
    this->expect(TokenType::OpenParen, "Expected open parenthesis");
    std::vector<Expr*> args;

    if (this->at().getType() != TokenType::CloseParen) {
        args = this->parse_arguments_list();
    }

    this->expect(TokenType::CloseParen, "Missing closing parenthesis inside arguments list");
    return args;
}


std::vector<Expr*> Parser::parse_arguments_list() {
    std::vector<Expr*> args;
    args.push_back(this->parse_assignment_expr());

    while (this->at().getType() == TokenType::Comma) {
        this->eat();
        args.push_back(this->parse_assignment_expr());
    }
    return args;
}
