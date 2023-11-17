#include "Interpreter.h"

RuntimeVal* Interpreter::eval_identifer(IdentifierExpr* ident, Environment* env) {
    RuntimeVal* val = env->lookupVar(ident->symbol);
    return val;
}

RuntimeVal* Interpreter::eval_assignment(AssignmentExpr* node, Environment* env ) {
    if (node->assigne->kind != NodeType::Identifier) {
        throw std::runtime_error("Invalid LHS inside assignment expr");
    }

    IdentifierExpr* ident = dynamic_cast<IdentifierExpr*>(node->assigne);
    const std::string varname = ident->symbol;
    return env->assignVar(varname, Interpreter::evaluate(node->value, env));
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
            result = Interpreter::evaluate(stmt, scope);
            if (result->type == ValueType::ReturnValue) {
                return result; 
            }
        }

        return result;
    }
    throw std::runtime_error("Cannot call value that is not a function");
}

RuntimeVal* Interpreter::eval_unary_expr(UnaryExpr* expr, Environment* env) {
    RuntimeVal* rightValue = Interpreter::evaluate(expr->right, env);

    if (expr->op == "!") {
        if (rightValue->type == ValueType::NumberValue) {
          NumberVal* number = dynamic_cast<NumberVal*>(rightValue);
          return new NumberVal(!number->value);
        }
    }

    std::cerr << "Unsupported unary operator: " << expr->op << std::endl;
    std::exit(1);
}

RuntimeVal*  Interpreter::eval_binary_expr(BinaryExpr* binop, Environment* env) {
    RuntimeVal* lhs = Interpreter::evaluate(binop->left, env);
    RuntimeVal* rhs = Interpreter::evaluate(binop->right, env);

    if (lhs->type == ValueType::NumberValue && rhs->type == ValueType::NumberValue) {
        NumberVal* leftNumber = dynamic_cast<NumberVal*>(lhs);
        NumberVal* rightNumber = dynamic_cast<NumberVal*>(rhs);

        if (binop->binaryOperator == "+") {
            return new NumberVal(leftNumber->value + rightNumber->value);
        }
        else if (binop->binaryOperator == "-") {
            return new NumberVal(leftNumber->value - rightNumber->value);
        }
        else if (binop->binaryOperator == "*") {
            return new NumberVal(leftNumber->value * rightNumber->value);
        }
        else if (binop->binaryOperator == ">") {
            return new NumberVal(leftNumber->value > rightNumber->value);
        }
        else if (binop->binaryOperator == "<") {
            return new NumberVal(leftNumber->value < rightNumber->value);
        }
        else if (binop->binaryOperator == ">=") {
            return new NumberVal(leftNumber->value >= rightNumber->value);
        }
        else if (binop->binaryOperator == ">=") {
            return new NumberVal(leftNumber->value >= rightNumber->value);
        }
        else if (binop->binaryOperator == "<=") {
            return new NumberVal(leftNumber->value >= rightNumber->value);
        }
        else if (binop->binaryOperator == "==") {
            return new NumberVal(leftNumber->value == rightNumber->value);
        }
        else if (binop->binaryOperator == "!=") {
            return new NumberVal(leftNumber->value != rightNumber->value);
        }
        else if (binop->binaryOperator == "/") {
            if (rightNumber->value != 0) {
                return  new NumberVal(leftNumber->value / rightNumber->value);
            }
            else {
                std::cerr << "Division by zero error" << std::endl;
                std::exit(1);
            }
        }
        else if (binop->binaryOperator == "%") {
            if (rightNumber->value != 0) {
                return new NumberVal(fmod(leftNumber->value, rightNumber->value));
            }
            else {
                std::cerr << "Modulo by zero error" << std::endl;
                std::exit(1);
            }
        }
    }
    return new NullVal;
}