#include "Interpreter.h"


std::unique_ptr<RuntimeVal> Interpreter::eval_identifer(std::unique_ptr<IdentifierExpr> ident, Environment* env) {
    auto val = env->lookupVar(ident->symbol);
    return val->clone();
}

std::unique_ptr<RuntimeVal> Interpreter::eval_assignment(std::unique_ptr<AssignmentExpr> node, Environment* env) {
    if (node->assigne->kind != NodeType::Identifier) {
        throw std::runtime_error("Invalid LHS inside assignment expr");
    }

    IdentifierExpr* ident = dynamic_cast<IdentifierExpr*>(node->assigne.get());
    const std::string varname = ident->symbol;

    // Przekazuj std::unique_ptr bez używania clone()
    return env->assignVar(varname, Interpreter::evaluate(std::move(node->value), env));
}


// std::unique_ptr<RuntimeVal> Interpreter::eval_call_expr(std::unique_ptr<CallExpr> expr, Environment* env) {
//     std::vector<std::unique_ptr<RuntimeVal>> args;
//     
//     for (auto& arg : expr->args) {
//         args.push_back(Interpreter::evaluate(std::move(arg), env)->clone());
//     }
//     
//     auto fn = Interpreter::evaluate(std::move(expr->caller), env);
//
//     if (fn->type == ValueType::NativeFunction) {
//         auto nativeFn = dynamic_cast<NativeFnVal*>(fn.get());
//         return nativeFn->call(args, env);
//     }
//
//     if (fn->type == ValueType::Function) {
//         auto func = dynamic_cast<FnVal*>(fn.get());
//         Environment* scope(func->declarationEnv);
//
//         for (size_t i = 0; i < func->parameters.size(); ++i) {
//             if (i < args.size()) {
//                 scope->declareVar(func->parameters[i], std::move(args[i]), false);
//             }
//         }
//         std::unique_ptr<RuntimeVal> result = std::make_unique<NullVal>();
//
//         for (auto& stmt : func->body) {
//             result = Interpreter::evaluate(std::move(stmt), scope);
//             if (result->type == ValueType::ReturnValue) {
//                 return result; 
//             }
//         }
//
//         return result;
//     }
//     throw std::runtime_error("Cannot call value that is not a function");
// }

std::unique_ptr<RuntimeVal> Interpreter::eval_unary_expr(std::unique_ptr<UnaryExpr> expr, Environment* env) {
    auto rightValue = Interpreter::evaluate(std::move(expr->right), env);

    if (expr->op == "!") {
        if (rightValue->type == ValueType::NumberValue) {
            auto number = dynamic_cast<NumberVal*>(rightValue.get());
            return std::make_unique<NumberVal>(!number->value);
        }
    }

    std::cerr << "Unsupported unary operator: " << expr->op << std::endl;
    std::exit(1);
}


std::unique_ptr<RuntimeVal> Interpreter::eval_binary_expr(std::unique_ptr<BinaryExpr> binop, Environment* env) {
    auto lhs = Interpreter::evaluate(std::move(binop->left), env);
    auto rhs = Interpreter::evaluate(std::move(binop->right), env);

    if (lhs->type == ValueType::NumberValue && rhs->type == ValueType::NumberValue) {
        auto leftNumber = dynamic_cast<NumberVal*>(lhs.get());
        auto rightNumber = dynamic_cast<NumberVal*>(rhs.get());

        if (binop->binaryOperator == "+") {
            return std::make_unique<NumberVal>(leftNumber->value + rightNumber->value);
        } else if (binop->binaryOperator == "-") {
            return std::make_unique<NumberVal>(leftNumber->value - rightNumber->value);
        } else if (binop->binaryOperator == "*") {
            return std::make_unique<NumberVal>(leftNumber->value * rightNumber->value);
        }

    }

    return std::make_unique<NullVal>();
}
