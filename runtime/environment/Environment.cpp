#include "Environment.h"
#include "../values/Values.h"


Environment::Environment(Environment* parentEnv) : parent(parentEnv) {}


RuntimeVal* Environment::declareVar(const std::string& varName, RuntimeVal* value, bool isConst) {
    if (variables.find(varName) != variables.end()) {
       std::cerr << "Cannot declare variable " << varName << ". It is already defined.";
       std::exit(1);
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
      std::cerr << "Cannot reassign to variable " << varName <<" as it was declared constant.";
      std::exit(1);
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
        std::cerr << "Cannot resolve '" << varName << "' as it does not exist.";
        std::exit(1);
    }

    return parent->resolve(varName);
}


bool Environment::isConstant(const std::string& varname) {
    return constants.find(varname) != constants.end();
}


NativeFnVal::NativeFnVal(FunctionType c) : call(c) {
    type = ValueType::NativeFunction;
}


FnVal::FnVal(std::string n, std::vector<std::string> p, Environment* d, std::vector<Stmt*> b) : name(n), parameters(p), declarationEnv(d), body(b) {
    type = ValueType::Function;
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

    return new NullVal;
}


RuntimeVal* exitFunction(const std::vector<RuntimeVal*> args, Environment* env) {
    exit(1);
    return new NullVal;
}


void Environment::createGlobalEnv() {
    this->declareVar("true", new BooleanVal(true), true);
    this->declareVar("false", new BooleanVal(false), true);
    this->declareVar("null", new NullVal(), true);
    this->declareVar("print", new NativeFnVal(printFunction), true);
    this->declareVar("exit", new NativeFnVal(exitFunction), true);
}
