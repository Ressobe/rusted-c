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


RuntimeVal* EvaluateStatement::eval_if_statement(IfStatement* ifStmt, Environment* env) {
    RuntimeVal* conditionValue = Interpreter::evaluate(ifStmt->condition, env);

    if (conditionValue->type == ValueType::NumberValue) {
        NumberVal* numCondition = dynamic_cast<NumberVal*>(conditionValue);

        if (numCondition->value != 0) {
            // Evaluate the "if" body when the condition is true
            return Interpreter::evaluate(ifStmt->ifBody, env);
        } else if (ifStmt->elseBody) {
            // Evaluate the "else" body when the condition is false, if it exists
            return Interpreter::evaluate(ifStmt->elseBody, env);
        }
    } else {
        std::cerr << "If statement condition must evaluate to a numeric value." << std::endl;
        std::exit(1);
    }

    return NullVal::MK_NULL();
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
