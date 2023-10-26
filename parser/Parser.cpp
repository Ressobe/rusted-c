#include "Parser.h"


bool Parser::not_eof() {
    return tokens[0].getType() != TokenType::EOFToken;
}

Token Parser::at() {
    return tokens[0];
}

Token Parser::eat() {
    Token prev = tokens[0];
    tokens.erase(tokens.begin());
    return prev;
}

Token Parser::expect(TokenType type, const std::string& err) {
    Token prev = this->eat();
    if (prev.getType() != type) {
        std::cerr << "Parser Error:\n" << err << prev.getValue() << " - Expecting: " << static_cast<int>(type) << std::endl;
        std::exit(1);
    }
    return prev;
}

Stmt* Parser::parse_stmt() {
    if (at().getType() == TokenType::Let || at().getType() == TokenType::Const) {
        return parse_var_declaration();
    }

    if (at().getType() == TokenType::Func) {
        return parse_function_declaration();
    }

    return parse_expr();
}


Expr* Parser::parse_expr() {
    return parse_assignment_expr();
}


Expr* Parser::parse_primary_expr() {
    TokenType tk = at().getType();
    Expr* value = nullptr;

    switch (tk) {
		case TokenType::Identifier:
			value = new IdentifierExpr(eat().getValue());
            break;
		case TokenType::Number:
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

    return value;
}

Program Parser::produceAST(const std::string& sourceCode) {
    Lexer lexer(sourceCode);
    this->tokens = lexer.tokenize();

    Program program;
    program.kind = NodeType::Program;

    while (not_eof()) {
        program.body.push_back(parse_stmt());
    }

    return program;
}

Stmt* Parser::parse_var_declaration() {
    bool isConstant = this->eat().getType() == TokenType::Const;
    std::string identifier = expect(TokenType::Identifier, "Expected identifier name following let | const keywords.").getValue();

    if (this->at().getType() == TokenType::Semicolon) {
        this->eat();
        if (isConstant) {
            throw "Must assign value to constant expression. No value provided.";
        }

        Stmt* declaration = new VarDeclaration(false, identifier);
        return declaration;
    }

    expect(TokenType::Equals, "Expected equals token following identifier in var declaration.");

    Expr* value = parse_expr();
    Stmt* declaration = new VarDeclaration(isConstant, identifier, value);

    expect(TokenType::Semicolon, "Variable declaration statement must end with semicolon.");

    return declaration;
}

Stmt* Parser::parse_function_declaration() {
    this->eat();
    std::string name = expect(TokenType::Identifier, "Expected function name following fn keyword").getValue();
    std::vector<Expr*> args = parse_args();

    std::vector<std::string> params;

    for (auto arg : args) {
            if (arg->kind != NodeType::Identifier) {
                std::cout << "Inside function declaration expected parameters to be of type string." << std::endl;
                std::exit(1);
            }
            params.push_back(static_cast<IdentifierExpr*>(arg)->symbol);
    }

    expect(TokenType::OpenBrace, "Expected function body following declaration");

    std::vector<Stmt*> body;

	while (at().getType() != TokenType::EOFToken && at().getType() != TokenType::CloseBrace) {
		body.push_back(parse_stmt());
	}

    expect(TokenType::CloseBrace, "Closing brace expected inside function declaration");

    FunctionDeclaration* fn = new FunctionDeclaration(params, name, body);

    return fn;
}

Expr* Parser::parse_additive_expr() {
    Expr* left = parse_multiplicative_expr();

    while (at().getValue() == "+" || at().getValue() == "-") {
        std::string binaryOperator = eat().getValue();
        Expr* right = parse_multiplicative_expr();
        left = new BinaryExpr(left, right, binaryOperator);
    }

    return left;
}

Expr* Parser::parse_multiplicative_expr() {
    Expr* left = parse_call_member_expr();

    while (at().getValue() == "/" || at().getValue() == "*" || at().getValue() == "%") {
        std::string binaryOperator = eat().getValue();
        Expr* right = parse_primary_expr();
        left = new BinaryExpr(left, right, binaryOperator);
    }

    return left;
}


Expr* Parser::parse_assignment_expr() {
    Expr* left = this->parse_additive_expr();

    if (this->at().getType() == TokenType::Equals) {
        this->eat();
        Expr* value = parse_assignment_expr();

        // Check for a semicolon
        if (this->at().getType() != TokenType::Semicolon) {
            // Raise an error if a semicolon is missing
            std::cerr << "Expected a semicolon at the end of the assignment expression." << std::endl;
            std::exit(1);
        }
        
        // Consume the semicolon
        this->eat();

        Expr* assignmentExpr = new AssignmentExpr(left, value);

        return assignmentExpr;
    }

    return left;

}

Expr* Parser::parse_call_member_expr() {
    Expr* caller = this->parse_primary_expr();

    while (this->at().getType() == TokenType::OpenParen) {
        this->eat(); // Consume the open parenthesis
        std::vector<Expr*> arguments;

        // Check for the closing parenthesis or arguments
        while (this->at().getType() != TokenType::CloseParen) {
            // Parse and add each argument to the vector
            arguments.push_back(this->parse_expr());

            // If a comma follows, consume it and continue to the next argument
            if (this->at().getType() == TokenType::Comma) {
                this->eat();
            }
        }


        expect(TokenType::CloseParen, "Expected a closing parenthesis in the function call.");
        expect(TokenType::Semicolon, "Call expresion  must end with semicolon.");


        // Create the call expression
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

