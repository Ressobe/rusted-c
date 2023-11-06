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

Stmt* Parser::parse_stmt() {
    std::cout << "parse_stmt" << std::endl;

    if (at().getType() == TokenType::Let || at().getType() == TokenType::Const) {
        return parse_var_declaration();        
    }

    if (at().getType() == TokenType::Func) {
        return parse_function_declaration();
    } 

    if (at().getType() == TokenType::If) {
        return parse_if_statement();
    }

    if (at().getType() == TokenType::Return) {
        Stmt* returnStmt = parse_return_statement();        
        expect(TokenType::Semicolon, "Var declaration must end with a semicolon.");
        return returnStmt;
    }


    return parse_expr();
}

Stmt* Parser::parse_return_statement() {
    eat(); // Consume the "return" keyword

    if (at().getType() == TokenType::Semicolon) {
        // Return statement without a value
        eat(); // Consume the semicolon
        return new ReturnStatement(nullptr);
    } else {
        // Return statement with a value
        Expr* value = parse_expr();
        expect(TokenType::Semicolon, "Return statement must end with a semicolon.");
        return new ReturnStatement(value);
    }
}

Stmt* Parser::parse_if_statement() {
    std::cout << "parse_if_statement" << std::endl;

    eat(); // Consume the "if" keyword
           
    expect(TokenType::OpenParen, "Expected '(' after 'if'");

    Expr* condition = parse_comprasion_expr();

    expect(TokenType::CloseParen, "Expected ')' after 'if' condition");


    expect(TokenType::OpenBrace, "Expected '{' open 'if' body");
    Stmt* ifBody = parse_stmt(); // Parse the body of the "if" statement
    expect(TokenType::CloseBrace, "Expected '}' close 'if' body");

    Stmt* elseBody = nullptr;

    if (at().getType() == TokenType::Else) {

        eat(); // Consume the "else" keyword
               
        expect(TokenType::OpenBrace, "Expected '{' open 'else' body");
        elseBody = parse_stmt(); // Parse the body of the "else" statement
        expect(TokenType::CloseBrace, "Expected '}' close 'else' body");
    }

    return new IfStatement(condition, ifBody, elseBody);
}


Expr* Parser::parse_expr() {

    
    std::cout << "parse_expr" << std::endl;

    return parse_assignment_expr();
}

Expr* Parser::parse_comprasion_expr() {
    std::cout << "parse_comprasion_expr" << std::endl;

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
    std::cout << "parse_primary_expr" << std::endl;

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

Program Parser::produceAST(std::vector<Token> tokens) {
    std::cout << "produceAST" << std::endl;

    this->tokens = tokens;

    Program program;
    program.kind = NodeType::Program;

    while (!eof()) {
        program.body.push_back(parse_stmt());
    }

    return program;
}


Stmt* Parser::parse_var_declaration() {
    std::cout << "parse_var_declaration" << std::endl;

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

    Expr* value = this->parse_expr();
    Stmt* declaration = new VarDeclaration(isConstant, identifier, value);

    expect(TokenType::Semicolon, "Var declaration must end with a semicolon.");


    return declaration;
}



Stmt* Parser::parse_function_declaration() {
    std::cout << "parse_function_declaration" << std::endl;

    this->eat();
    std::string name = this->expect(TokenType::Identifier, "Expected function name following fn keyword").getValue();
    std::vector<Expr*> args = this->parse_args();

    std::vector<std::string> params;

    for (auto arg : args) {
        if (arg->kind == NodeType::Identifier) {
            params.push_back(static_cast<IdentifierExpr*>(arg)->symbol);
        } else {
            std::cerr << "Inside function declaration expected parameters to be of type Identifier." << std::endl;
            std::exit(1);
        }
    }

    expect(TokenType::OpenBrace, "Expected function body following declaration");

    std::vector<Stmt*> body;

    while (this->at().getType() != TokenType::EOFToken && this->at().getType() != TokenType::CloseBrace) {
        body.push_back(parse_stmt());
    }

    expect(TokenType::CloseBrace, "Closing brace expected inside function declaration");

    FunctionDeclaration* fn = new FunctionDeclaration(params, name, body);

    return fn;
}


Expr* Parser::parse_additive_expr() {
    std::cout << "parse_additive_expr" << std::endl;
    Expr* left = this->parse_multiplicative_expr();

    while (this->at().getValue() == "+" || this->at().getValue() == "-") {
        std::string binaryOperator = this->eat().getValue();
        Expr* right = this->parse_multiplicative_expr();
        left = new BinaryExpr(left, right, binaryOperator);
    }

    return left;
}

Expr* Parser::parse_multiplicative_expr() {
    std::cout << "parse_multiplicative_expr" << std::endl;

    Expr* left = this->parse_call_member_expr();

    while (at().getValue() == "/" || at().getValue() == "*" || at().getValue() == "%") {
        std::string binaryOperator = this->eat().getValue();
        Expr* right = this->parse_primary_expr();
        left = new BinaryExpr(left, right, binaryOperator);
    }

    return left;
}


Expr* Parser::parse_assignment_expr() {
    std::cout << "parse_assigment_expr" << std::endl;

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
    std::cout << "parse_call_member_expr" << std::endl;

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

        // Oczekiwanie na srednik po wywołaniu funkcji, ale jest opcjonalny
        if (this->at().getType() == TokenType::Semicolon) {
            this->eat(); // Konsumowanie srednika
        } else if (this->at().getType() != TokenType::Semicolon && this->at().getType() != TokenType::CloseParen) {
            std::cerr << "Parser Error:\nExpected a semicolon (;) at the end of the line or a closing parenthesis after function call." << std::endl;
            std::exit(1);
        }



        // Create the call expression
        caller = new CallExpr(caller, arguments);
    }

    return caller;
}

Expr* Parser::parse_call_expr(Expr* caller) {
    std::cout << "parse_call_expr" << std::endl;
    Expr* call_expr = new CallExpr(caller,this->parse_args());

    while (this->at().getType() == TokenType::OpenParen) {
        call_expr = parse_call_expr(call_expr);
    }

    return call_expr;
}


std::vector<Expr*> Parser::parse_args() {
    std::cout << "parse_args" << std::endl;

    this->expect(TokenType::OpenParen, "Expected open parenthesis");
    std::vector<Expr*> args;

    if (this->at().getType() != TokenType::CloseParen) {
        args = this->parse_arguments_list();
    }

    this->expect(TokenType::CloseParen, "Missing closing parenthesis inside arguments list");
    return args;
}

std::vector<Expr*> Parser::parse_arguments_list() {
    std::cout << "parse_arguments_list" << std::endl;

    std::vector<Expr*> args;
    args.push_back(this->parse_assignment_expr());

    while (this->at().getType() == TokenType::Comma) {
        this->eat();
        args.push_back(this->parse_assignment_expr());
    }

    return args;
}

