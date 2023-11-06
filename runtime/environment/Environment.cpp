#include "Environment.h"
#include "../values/Values.h"

Environment::Environment(Environment* parentEnv) : parent(parentEnv) {}


RuntimeVal* Environment::declareVar(const std::string& varName, RuntimeVal* value, bool isConst) {
    if (variables.find(varName) != variables.end()) {
        throw "Cannot declare variable " + varName + ". It is already defined.";
    }

    variables[varName] = value;

    if (isConst) {
        constants.insert(varName);
    }

    return value;
}

RuntimeVal* Environment::assignVar(const std::string& varName, RuntimeVal* value) {
    Environment* env = resolve(varName);

    if (isConstant(varName)) { 
        throw "Cannot reassign to variable " + varName + " as it was declared constant.";
    }

    env->variables[varName] = value;
    return value;
}

RuntimeVal* Environment::lookupVar(const std::string& varName) {
    Environment* env = resolve(varName);
    return env->variables[varName];
}

Environment* Environment::resolve(const std::string& varName) {
    if (variables.find(varName) != variables.end()) {
        return this;
    }

    if (parent == nullptr) {
        throw "Cannot resolve '" + varName + "' as it does not exist.";
    }

    return parent->resolve(varName);
}

bool Environment::isConstant(const std::string& varname) {
    return constants.find(varname) != constants.end();
}

NativeFnVal::NativeFnVal(FunctionType c) : call(c)
{
    type = ValueType::NativeFunction;
}

FnVal::FnVal(std::string n, std::vector<std::string> p, Environment* d, std::vector<Stmt*> b) : name(n), parameters(p), declarationEnv(d), body(b)
{
    type = ValueType::Function;
}


NativeFnVal* NativeFnVal::MK_NATIVE_FN(FunctionType call)
{
    return new NativeFnVal(call);
}

RuntimeVal* printFunction(const std::vector<RuntimeVal*> args, Environment* env) {

    for (auto arg : args) {
        if (arg->type == ValueType::BooleanValue) {
            std::cout << (static_cast<BooleanVal*>(arg)->value ? "true" : "false") << " ";
        }
        else if (arg->type == ValueType::NumberValue) {
            std::cout << static_cast<NumberVal*>(arg)->value << " ";
        }
    }

    std::cout << std::endl;

    return NullVal::MK_NULL();
}

RuntimeVal* exitFunction(const std::vector<RuntimeVal*> args, Environment* env) {
    exit(1);
    return NullVal::MK_NULL();
}

void Environment::createGlobalEnv() {

    // Create Default Global Environment
    this->declareVar("true", BooleanVal::MK_BOOL(true), true);
    this->declareVar("false", BooleanVal::MK_BOOL(false), true);
    this->declareVar("null", NullVal::MK_NULL(), true);
    this->declareVar("print", NativeFnVal::MK_NATIVE_FN(printFunction), true);
    this->declareVar("exit", NativeFnVal::MK_NATIVE_FN(exitFunction), true);
}
