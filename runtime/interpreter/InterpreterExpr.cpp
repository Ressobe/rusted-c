#include "Interpreter.h"


RuntimeVal* Interpreter::eval_identifer(IdentifierExpr* ident, Environment* env) {
    RuntimeVal* val = env->lookupVar(ident->symbol);
    return val;
}


RuntimeVal* Interpreter::eval_assignment(AssignmentExpr* node, Environment* env) {

    if (node->assigne->kind == NodeType::MemberAccessExpr) {
        return eval_member_access_assignment(dynamic_cast<MemberAccessExpr*>(node->assigne.get()), node->value.get(), env);
    }

    if (node->assigne->kind != NodeType::Identifier) {
        throw std::runtime_error("Invalid LHS inside assignment expr");
    }

    IdentifierExpr* ident = dynamic_cast<IdentifierExpr*>(node->assigne.get());
    const std::string varname = ident->symbol;
    return env->assignVar(varname, Interpreter::evaluate(node->value.get(), env));
}


RuntimeVal* Interpreter::eval_call_expr(CallExpr* expr, Environment* env) {
    std::vector<RuntimeVal*> args;

         
    for (auto& arg : expr->args) {
        args.push_back(Interpreter::evaluate(arg.get(), env));
    }
    
    RuntimeVal* caller = Interpreter::evaluate(expr->caller.get(), env);


    if (caller->type == ValueType::StructValue) {
        StructVal* structVal = dynamic_cast<StructVal*>(caller);
        if (!structVal->isDeclaration) {
            std::cout << "Cannot create struct instance from other struct instance" << std::endl;
            std::exit(1);
        }

        StructVal* newStructInstance = new StructVal(structVal->structName, false);

        for (const auto& field : structVal->fields) {
            newStructInstance->addField(field.first, field.second);
        }

        auto it = newStructInstance->fields.begin();
        for (const auto& arg : args) {
            if (it != newStructInstance->fields.end()) {
                it->second = arg;
                ++it;
            }
        }

        return newStructInstance;
    }

    if (caller->type == ValueType::NativeFunction) {
        NativeFnVal* nativeFn = dynamic_cast<NativeFnVal*>(caller);
        return nativeFn->call(args, env);
    }

    if (caller->type == ValueType::Function) {
        FnVal* func = dynamic_cast<FnVal*>(caller);
        Environment* functionEnv = new Environment(env);

        for (size_t i = 0; i < func->parameters.size(); ++i) {
            if (i < args.size()) {
                functionEnv->declareVar(func->parameters[i], args[i], false);
            }
        }
        RuntimeVal* result = new NullVal;

        for (Stmt* stmt : func->body) {
            result = Interpreter::evaluate(stmt, functionEnv);
            if (result->type == ValueType::ReturnValue) {
                return result; 
                delete functionEnv;
            }
        }


        delete functionEnv;
        return result;
    }
    throw std::runtime_error("Cannot call value that is not a function");
}

RuntimeVal* Interpreter::eval_member_access(MemberAccessExpr* memberAccess, Environment* env) {
    RuntimeVal* object = evaluate(memberAccess->object.get(), env);

    // Sprawdzenie, czy obiekt jest rzeczywiœcie obiektem struktury
    if (object->type != ValueType::StructValue) {
        std::cerr << "Error: Member access is only supported for structs." << std::endl;
        std::exit(1);
    }

    StructVal* structVal = dynamic_cast<StructVal*>(object);
    return structVal->getField(memberAccess->memberName);
}

RuntimeVal* Interpreter::eval_member_access_assignment(MemberAccessExpr* memberAccessExpr, Expr* valueExpr, Environment* env) {
    // Ewaluacja obiektu, którego dotyczy dostêp
    RuntimeVal* object = evaluate(memberAccessExpr->object.get(), env);
    // Sprawdzenie, czy obiekt jest rzeczywiœcie obiektem struktury
    if (object->type != ValueType::StructValue) {
        std::cerr << "Error: Member access is only supported for structs." << std::endl;
        std::exit(1);
    }
    StructVal* structVal = dynamic_cast<StructVal*>(object);

    // Operacja przypisania do pola struktury
    RuntimeVal* value = evaluate(valueExpr, env);

    // Przypisanie wartoœci do pola struktury
    structVal->setField(memberAccessExpr->memberName, value);

    // Zwróæ wartoœæ przypisan¹
    return value;
}

RuntimeVal* Interpreter::eval_unary_expr(UnaryExpr* expr, Environment* env) {
    RuntimeVal* rightValue = Interpreter::evaluate(expr->right.get(), env);

    if (expr->op == "!") {
        if (rightValue->type == ValueType::NumberValue) {
          NumberVal* number = dynamic_cast<NumberVal*>(rightValue);
          return new NumberVal(!number->value);
        }
    }

    if (expr->op == "-") {
        if (rightValue->type == ValueType::NumberValue) {
          NumberVal* number = dynamic_cast<NumberVal*>(rightValue);
          return new NumberVal(-number->value);
        }
    }

    std::cerr << "Unsupported unary operator: " << expr->op << std::endl;
    std::exit(1);
}

RuntimeVal*  Interpreter::eval_binary_expr(BinaryExpr* binop, Environment* env) {
    RuntimeVal* lhs = Interpreter::evaluate(binop->left.get(), env);
    RuntimeVal* rhs = Interpreter::evaluate(binop->right.get(), env);

    if (lhs->type == ValueType::ReturnValue) {
        lhs = dynamic_cast<ReturnValue*>(lhs)->value;
    }

    if (rhs->type == ValueType::ReturnValue) {
        rhs = dynamic_cast<ReturnValue*>(rhs)->value;
    }


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
        else if (binop->binaryOperator == "<=") {
            return new NumberVal(leftNumber->value <= rightNumber->value);
        }
        else if (binop->binaryOperator == ">=") {
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
