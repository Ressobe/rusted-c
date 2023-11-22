#include "Parser.h"

using ExprPtr = std::unique_ptr<Expr>;
using StmtPtr = std::unique_ptr<Stmt>;


StmtPtr Parser::parse_stmt() {
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
        return parse_return_statement();
    }

    return parse_expr();
}

StmtPtr Parser::parse_while_statement() {
    eat();

    expect(TokenType::OpenParen, "Expected '(' after 'while'");

    ExprPtr condition = parse_comprasion_expr();    

    expect(TokenType::CloseParen, "Expected ')' after 'while' condition");    

    expect(TokenType::OpenBrace, "Expected '{' open 'while' body");

    std::vector<StmtPtr> loopBody;
    while (at().getType() != TokenType::CloseBrace) {
          loopBody.push_back(parse_stmt());
    }

    expect(TokenType::CloseBrace, "Expected '}' close 'while' body");

    return std::make_unique<WhileLoop>(std::move(condition), std::move(loopBody));
}

StmtPtr Parser::parse_return_statement() {
    eat(); // Consume the "return" keyword

    if (at().getType() == TokenType::Semicolon) {
        // Return statement without a value
        eat(); // Consume the semicolon
        return std::make_unique<ReturnStatement>(nullptr);
    } else {
        // Return statement with a value
        std::unique_ptr<Expr> value = parse_expr();
        expect(TokenType::Semicolon, "Return statement must end with a semicolon.");
        return std::make_unique<ReturnStatement>(std::move(value));
    }
}

StmtPtr Parser::parse_if_statement() {
    eat(); // Consume the "if" keyword
    expect(TokenType::OpenParen, "Expected '(' after 'if'");

    ExprPtr condition = parse_comprasion_expr();

    expect(TokenType::CloseParen, "Expected ')' after 'if' condition");

    std::vector<StmtPtr> ifBody;

    expect(TokenType::OpenBrace, "Expected '{' open 'if' body");

    while (at().getType() != TokenType::CloseBrace) {
            ifBody.push_back(parse_stmt());
    }

    expect(TokenType::CloseBrace, "Expected '}' close 'if' body");
    
    std::vector<StmtPtr> elseBody;    

    if (at().getType() == TokenType::Else) {
        eat();                

        expect(TokenType::OpenBrace, "Expected '{' open 'else' body");
        while (at().getType() != TokenType::CloseBrace) {
                    elseBody.push_back(parse_stmt());
        }                
        expect(TokenType::CloseBrace, "Expected '}' close 'else' body");
    }

    return std::make_unique<IfStatement>(std::move(condition), std::move(ifBody), std::move(elseBody));
}

StmtPtr Parser::parse_var_declaration() {
    bool isConstant = this->eat().getType() == TokenType::Const;

    std::string identifier = expect(TokenType::Identifier, "Expected identifier name following let | const keywords.").getValue();

    if (this->at().getType() == TokenType::Semicolon) {
        this->eat();
        if (isConstant) {
          std::cerr << "Must assign value to constant expression. No value provided." << std::endl;            
          std::exit(1);
        }

        return std::make_unique<VarDeclaration>(false, identifier);
    }

    expect(TokenType::Equals, "Expected equals token following identifier in var declaration.");

    ExprPtr value = this->parse_expr();

    expect(TokenType::Semicolon, "Var declaration must end with a semicolon.");


    return std::make_unique<VarDeclaration>(isConstant, identifier, std::move(value));
}

StmtPtr Parser::parse_function_declaration() {
    this->eat();
    std::string name = this->expect(TokenType::Identifier, "Expected function name following fn keyword").getValue();
    std::vector<ExprPtr> args = this->parse_args();

    std::vector<std::string> params;

    for (auto& arg : args) {
        if (arg->kind == NodeType::Identifier) {
            params.push_back(static_cast<IdentifierExpr*>(arg.get())->symbol);
        } else {
            std::cerr << "Inside function declaration expected parameters to be of type Identifier." << std::endl;
            std::exit(1);
        }
    }

    expect(TokenType::OpenBrace, "Expected function body following declaration");

    std::vector<Stmt*> body;

    while (this->at().getType() != TokenType::EOFToken && this->at().getType() != TokenType::CloseBrace) {
        body.push_back(parse_stmt().release());
    }

    expect(TokenType::CloseBrace, "Closing brace expected inside function declaration");

    return std::make_unique<FunctionDeclaration>(std::move(params), name, body);
}