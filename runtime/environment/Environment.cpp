#include "Environment.h"
#include "../values/Values.h"


Environment::Environment(Environment* parentEnv) : parent(parentEnv) {}


std::unique_ptr<RuntimeVal> Environment::declareVar(const std::string& varname, std::unique_ptr<RuntimeVal> value, bool constant) {
    if (variables.find(varname) != variables.end()) {
        throw std::runtime_error("Cannot declare variable " + varname + ". It is already defined.");
    }

    variables[varname] = std::move(value);
    if (constant) {
        constants.insert(varname);
    }

    // Zwracamy std::unique_ptr<RuntimeVal>, ponieważ zmienna jest przechowywana jako unikalny wskaźnik.
    return variables[varname]->clone();
}


std::unique_ptr<RuntimeVal> Environment::assignVar(const std::string& varname, std::unique_ptr<RuntimeVal> value) {
    Environment* env = resolve(varname);

    // Cannot assign to constant
    if (env->isConstant(varname)) {
        throw std::runtime_error("Cannot reassign to variable " + varname + " as it was declared constant.");
    }

    // Przenosimy wartość do zmiennej w środowisku.
    env->variables[varname] = std::move(value);

    // Zwracamy std::unique_ptr<RuntimeVal>, ponieważ zmienna jest przechowywana jako unikalny wskaźnik.
    return env->variables[varname]->clone();
}


std::unique_ptr<RuntimeVal> Environment::lookupVar(const std::string& varname) {
    Environment* env = resolve(varname);
    
    auto it = env->variables.find(varname);
    if (it != env->variables.end()) {
        // Zwracamy klon wartości, aby uniknąć bezpośredniego dostępu do obiektów w środowisku.
        return it->second->clone();
    } else {
        // Rzucamy wyjątek, jeśli zmienna nie istnieje w środowisku.
        throw std::runtime_error("Variable " + varname + " not found.");
    }
}


Environment* Environment::resolve(const std::string& varname) {
    if (variables.find(varname) != variables.end()) {
        return this;
    }

    if (parent == nullptr) {
        throw std::runtime_error("Cannot resolve '" + varname + "' as it does not exist.");
    }

    return parent->resolve(varname);
}


bool Environment::isConstant(const std::string& varname) {
    return constants.find(varname) != constants.end();
}


NativeFnVal::NativeFnVal(FunctionType c) : call(c) {
    type = ValueType::NativeFunction;
} 


std::unique_ptr<RuntimeVal> NativeFnVal::clone() const {
        return std::make_unique<NativeFnVal>(*this);
}


FnVal::FnVal(std::string n, std::vector<std::string> p, Environment* d, std::vector<Stmt*> b)
    : name(n), parameters(p), declarationEnv(d), body(std::move(b)) {
    type = ValueType::Function;
}


std::unique_ptr<RuntimeVal> FnVal::clone() const {
    return std::make_unique<FnVal>(*this);
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
    this->declareVar("true",  std::make_unique<BooleanVal>(true), true);
    this->declareVar("false", std::make_unique<BooleanVal>(false), true);
    this->declareVar("null",  std::make_unique<NullVal>(), true);
    this->declareVar("print", std::make_unique<NativeFnVal>(printFunction), true);
    this->declareVar("exit",  std::make_unique<NativeFnVal>(exitFunction), true);
}
