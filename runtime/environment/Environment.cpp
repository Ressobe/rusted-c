#include "Environment.h"
#include "../standard-library/BuiltinFunctions.h"


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
        throw std::runtime_error("Cannot reassign to variable " + varName + " as it was declared constant.");
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
       std::cerr << "Cannot resolve ' " <<   varName << " ' as it does not exist.";
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


FnVal::FnVal(std::string n, std::vector<std::string> p, Environment* d, std::vector<Stmt*> b)
    : name(n), parameters(std::move(p)), declarationEnv(d), body(std::move(b)) {
    type = ValueType::Function;
}


std::unique_ptr<RuntimeVal> NativeFnVal::clone() const {
        return std::make_unique<NativeFnVal>(*this);
}


std::unique_ptr<RuntimeVal> FnVal::clone() const {
   return std::make_unique<FnVal>(*this);
}


void Environment::createGlobalEnv() {
    this->declareVar("true", new BooleanVal(true), true);
    this->declareVar("false",new BooleanVal(false), true);
    this->declareVar("null", new NullVal(), true);
    this->declareVar("print", new NativeFnVal(printFunction), true);
    this->declareVar("exit", new NativeFnVal(exitFunction), true);
    this->declareVar("clear", new NativeFnVal(clearFunction), true);
    this->declareVar("sqrt", new NativeFnVal(sqrtFunction), true);
    this->declareVar("pow", new NativeFnVal(powFunction), true);
    this->declareVar("round", new NativeFnVal(roundFunction), true);
    this->declareVar("min", new NativeFnVal(minFunction), true);
    this->declareVar("max", new NativeFnVal(maxFunction), true);
    this->declareVar("input", new NativeFnVal(inputFunction), true);
    this->declareVar("num", new NativeFnVal(numberFunction), true);
    this->declareVar("len", new NativeFnVal(lenFunction), true);
}
