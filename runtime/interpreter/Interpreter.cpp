#include "Interpreter.h"


RuntimeVal*  Interpreter::eval_program(Program* program, Environment* env) {
    RuntimeVal* lastEvaluated = NullVal::MK_NULL();
    for (Stmt* statement : program->body) {
        lastEvaluated = evaluate(statement, env);
    }
    return lastEvaluated;
}

RuntimeVal* Interpreter::eval_binary_expr(BinaryExpr* binop, Environment* env) {
    RuntimeVal* lhs = evaluate(binop->left, env);
    RuntimeVal* rhs = evaluate(binop->right, env);

    if (lhs->type == ValueType::NumberValue && rhs->type == ValueType::NumberValue) {
        NumberVal* leftNumber = dynamic_cast<NumberVal*>(lhs);
        NumberVal* rightNumber = dynamic_cast<NumberVal*>(rhs);

        if (binop->binaryOperator == "+") {
            return NumberVal::MK_NUMBER(leftNumber->value + rightNumber->value);
        }
        else if (binop->binaryOperator == "-") {
            return NumberVal::MK_NUMBER(leftNumber->value - rightNumber->value);
        }
        else if (binop->binaryOperator == "*") {
            return NumberVal::MK_NUMBER(leftNumber->value * rightNumber->value);
        }
        else if (binop->binaryOperator == "/") {
            if (rightNumber->value != 0) {
                return  NumberVal::MK_NUMBER(leftNumber->value / rightNumber->value);
            }
            else {
                std::cerr << "Division by zero error" << std::endl;
                std::exit(1);
            }
        }
        else if (binop->binaryOperator == "%") {
            if (rightNumber->value != 0) {
                return NumberVal::MK_NUMBER(fmod(leftNumber->value, rightNumber->value));
            }
            else {
                std::cerr << "Modulo by zero error" << std::endl;
                std::exit(1);
            }
        }
    }

    return NullVal::MK_NULL();
}

RuntimeVal* Interpreter::eval_identifer(IdentifierExpr* ident, Environment* env) {
    RuntimeVal* val = env->lookupVar(ident->symbol);
    return val;
}

RuntimeVal* Interpreter::eval_var_declaration(VarDeclaration* declaration, Environment* env) {
    RuntimeVal* value = declaration->value
        ? evaluate(declaration->value, env)
        : NullVal::MK_NULL();

    return env->declareVar(declaration->identifier, value, declaration->constant);
}

RuntimeVal* Interpreter::eval_function_declaration(FunctionDeclaration* declaration, Environment* env)
{
    FnVal fn = FnVal(declaration->name, declaration->parameters, env, declaration->body);

    return env->declareVar(declaration->name, &fn, true);
}

RuntimeVal* Interpreter::eval_assignment(AssignmentExpr* node, Environment* env) {
    if (node->assigne->kind != NodeType::Identifier) {
        throw "Invalid LHS inside assignment expr";
    }

    IdentifierExpr* ident = dynamic_cast<IdentifierExpr*>(node->assigne);
    const std::string varname = ident->symbol;
    return env->assignVar(varname, evaluate(node->value, env));
}




RuntimeVal* Interpreter::evaluate(Stmt* astNode, Environment* env) {
    if (astNode->kind == NodeType::NumericLiteral) {
        NumericLiteral* numLiteral = dynamic_cast<NumericLiteral*>(astNode);
        return NumberVal::MK_NUMBER(numLiteral->value);
    }
    else if (astNode->kind == NodeType::Null) {
        return NullVal::MK_NULL();
    }
    else if (astNode->kind == NodeType::Identifier) {
        return eval_identifer(dynamic_cast<IdentifierExpr*>(astNode), env);
    }
    else if (astNode->kind == NodeType::BinaryExpr) {
        return eval_binary_expr(dynamic_cast<BinaryExpr*>(astNode), env);
    }
    else if (astNode->kind == NodeType::Program) {
        return eval_program(dynamic_cast<Program*>(astNode), env);
    }
    else if (astNode->kind == NodeType::VarDeclaration) {
        return eval_var_declaration(dynamic_cast<VarDeclaration*>(astNode), env);
    }
    else if (astNode->kind == NodeType::AssignmentExpr) {
        return eval_assignment(dynamic_cast<AssignmentExpr*>(astNode), env);
    }
    else if (astNode->kind == NodeType::CallExpr) {
        return eval_call_expr(dynamic_cast<CallExpr*>(astNode), env);
    }
    else if (astNode->kind == NodeType::FunctionDeclaration) {
        return eval_function_declaration(dynamic_cast<FunctionDeclaration*>(astNode), env);
    }


    else {
        std::cerr << "This AST Node has not yet been set up for interpretation." << std::endl;
        std::exit(1);
    }
}



RuntimeVal* Interpreter::eval_call_expr(CallExpr* expr, Environment* env) {
    std::vector<RuntimeVal*> args;
    
    for (auto arg : expr->args) {
        args.push_back(Interpreter::evaluate(arg, env));
    }
    
    RuntimeVal* fn = Interpreter::evaluate(expr->caller, env);

    if (fn->type == ValueType::NativeFunction) {
        NativeFnVal* nativeFn = dynamic_cast<NativeFnVal*>(fn);
        return nativeFn->call(args, env);
    }

    /*
    if (fn->type == ValueType::Function) {
        FnVal* func = dynamic_cast<FnVal*>(fn);
        Environment* scope(func->declarationEnv);

        for (size_t i = 0; i < func->parameters.size(); ++i) {
            if (i < args.size()) {
                scope->declareVar(func->parameters[i], args[i], false);
            }
        }

        RuntimeVal* result = NullVal::MK_NULL();
        for (Stmt* stmt : func->body) {
            result = evaluate(stmt, scope);
        }

        return result;
    }*/

    throw "Cannot call value that is not a function";
}

