#include "Parser.h"


Stmt* Parser::parse_stmt() {
    if (at().getType() == TokenType::Let || at().getType() == TokenType::Const) {
        return parse_var_declaration();        
    }

    if (at().getType() == TokenType::Func) {
        return parse_function_declaration();
    } 

    if (at().getType() == TokenType::If) {
        return parse_if_statement();
    }

    if (at().getType() == TokenType::While) {
      return parse_while_statement();
    }

    if (at().getType() == TokenType::Return) {
        Stmt* returnStmt = parse_return_statement();        
        expect(TokenType::Semicolon, "Var declaration must end with a semicolon.");
        return returnStmt;
    }

    return parse_expr();
}


Stmt* Parser::parse_while_statement() {
    eat();

    expect(TokenType::OpenParen, "Expected '(' after 'while'");

    Expr* condition = parse_comprasion_expr();    

    expect(TokenType::CloseParen, "Expected ')' after 'while' condition");    

    expect(TokenType::OpenBrace, "Expected '{' open 'while' body");

    std::vector<Stmt*> loopBody;
    while (at().getType() != TokenType::CloseBrace) {
          loopBody.push_back(parse_stmt());
    }

    expect(TokenType::CloseBrace, "Expected '}' close 'while' body");

    return new WhileLoop(condition, loopBody);
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
    eat(); // Consume the "if" keyword
    expect(TokenType::OpenParen, "Expected '(' after 'if'");

    Expr* condition = parse_comprasion_expr();

    expect(TokenType::CloseParen, "Expected ')' after 'if' condition");

    std::vector<Stmt*> ifBody;

    expect(TokenType::OpenBrace, "Expected '{' open 'if' body");

    while (at().getType() != TokenType::CloseBrace) {
            ifBody.push_back(parse_stmt());
    }

    expect(TokenType::CloseBrace, "Expected '}' close 'if' body");
    
    std::vector<Stmt*> elseBody;    

    if (at().getType() == TokenType::Else) {
        eat();                

        expect(TokenType::OpenBrace, "Expected '{' open 'else' body");
        while (at().getType() != TokenType::CloseBrace) {
                    elseBody.push_back(parse_stmt());
        }                
        expect(TokenType::CloseBrace, "Expected '}' close 'else' body");
    }

    return new IfStatement(condition, ifBody, elseBody);
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

    Expr* value = this->parse_expr();
    Stmt* declaration = new VarDeclaration(isConstant, identifier, value);

    expect(TokenType::Semicolon, "Var declaration must end with a semicolon.");

    return declaration;
}


Stmt* Parser::parse_function_declaration() {
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
