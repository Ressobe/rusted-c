#include "EvaluateStatement.h"


RuntimeVal* EvaluateStatement::eval_program(Program* program, Environment* env) {
    RuntimeVal* lastEvaluated = NullVal::MK_NULL();
    for (Stmt* statement : program->body) {
        lastEvaluated = Interpreter::evaluate(statement, env);
    }
    return lastEvaluated;
}

RuntimeVal* EvaluateStatement::eval_return_statement(ReturnStatement* stmt, Environment* env) {
    if (stmt->returnValue) {
        RuntimeVal* result  = Interpreter::evaluate(stmt->returnValue, env);
        return new ReturnValue(result);
    } else {
        return NullVal::MK_NULL();
    }
}

RuntimeVal* EvaluateStatement::eval_stmt_vector(const std::vector<Stmt*>& stmts, Environment* env) {
    RuntimeVal* result = nullptr;

    for (Stmt* stmt : stmts) {
        result = Interpreter::evaluate(stmt, env);

        // Sprawdź, czy wynik ewaluacji to return (zakończ jeśli tak)
        if (result->type == ValueType::ReturnValue) {
            return result;
        }
    }

    return result;
}


RuntimeVal* EvaluateStatement::eval_if_statement(IfStatement* ifStmt, Environment* env) {

    RuntimeVal* conditionValue = Interpreter::evaluate(ifStmt->condition, env);


    if (conditionValue->type == ValueType::NumberValue) {
        NumberVal* numCondition = dynamic_cast<NumberVal*>(conditionValue);

        if (numCondition->value != 0) {
            return EvaluateStatement::eval_stmt_vector(ifStmt->ifBody, env);
        } else if (ifStmt->elseBody.size() > 0) {
            return EvaluateStatement::eval_stmt_vector(ifStmt->elseBody, env);
        }
    } else {
        std::cerr << "If statement condition must evaluate to a numeric value." << std::endl;
        std::exit(1);
    }

    return NullVal::MK_NULL();
}

RuntimeVal* EvaluateStatement::eval_while_statement(WhileLoop* loop, Environment* env) {
    RuntimeVal* result = NullVal::MK_NULL();

    while (true) {
        // Ewaluacja warunku pętli
        RuntimeVal* conditionValue = Interpreter::evaluate(loop->condition, env);

        if (conditionValue->type == ValueType::NumberValue) {
            NumberVal* numCondition = dynamic_cast<NumberVal*>(conditionValue);

            if (numCondition->value != 0) {
                // Ewaluacja ciała pętli, wykonanie instrukcji wewnątrz pętli
                result = EvaluateStatement::eval_stmt_vector(loop->loopBody, env);
            } else {
                break;
            }
        } else {
            std::cerr << "While loop condition must evaluate to a numeric value." << std::endl;
            std::exit(1);
        }
    }
    return result;
}


RuntimeVal* EvaluateStatement::eval_var_declaration(VarDeclaration* declaration, Environment* env) {
    RuntimeVal* value = declaration->value
        ? Interpreter::evaluate(declaration->value, env)
        : NullVal::MK_NULL();

    return env->declareVar(declaration->identifier, value, declaration->constant);
}


RuntimeVal* EvaluateStatement::eval_function_declaration(FunctionDeclaration* declaration, Environment* env) {
    FnVal* fn = new FnVal(declaration->name, declaration->parameters, env, declaration->body);
    return env->declareVar(declaration->name, fn, true);
}
