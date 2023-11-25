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


RuntimeVal* printFunction(const std::vector<RuntimeVal*> args, Environment* env) {
    for (auto arg : args) {
        std::cout << arg->toString() << " ";
    }
    std::cout << std::endl;

    return new NullVal;
}

// User input
// input
//
// Math functions
// bin
//
// Change type
// num
// str
//
// String functions
// capitalize
// lower
//
// Return len of string or number of digits in number
// len

void clearScreen() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}


RuntimeVal* clearFunction(const std::vector<RuntimeVal*> args, Environment* env) {
    clearScreen();

    return new NullVal;
}


RuntimeVal* sqrtFunction(const std::vector<RuntimeVal*> args, Environment* env) {
  if (args.size() != 1) {
    std::cerr << "Wrong number of arguments in sqrt function" << std::endl;
    std::exit(1);
  }

  if (args[0]->type != ValueType::NumberValue) {
    std::cerr << "Wrong argument type for sqrt function" << std::endl;
    std::exit(1);
  }
  
  NumberVal* number = dynamic_cast<NumberVal*>(args[0]);

  return new NumberVal(sqrt(number->value));
}

RuntimeVal* powFunction(const std::vector<RuntimeVal*> args, Environment* env) {
  if (args.size() != 2) {
    std::cerr << "Wrong number of arguments in pow function" << std::endl;
    std::exit(1);
  }

  if (args[0]->type == ValueType::NumberValue && args[1]->type == ValueType::NumberValue) {
    return new NumberVal(pow(dynamic_cast<NumberVal*>(args[0])->value, dynamic_cast<NumberVal*>(args[1])->value));
  }
  else {
    std::cerr << "Wrong argument type for pow function" << std::endl;
    std::exit(1);
  }
}


RuntimeVal* roundFunction(const std::vector<RuntimeVal*> args, Environment* env) {
  if (args.size() != 1) {
    std::cerr << "Wrong number of arguments in round function" << std::endl;
    std::exit(1);
  }

  if (args[0]->type != ValueType::NumberValue) {
    std::cerr << "Wrong argument type for round function" << std::endl;
    std::exit(1);
  }

  return new NumberVal(round(dynamic_cast<NumberVal*>(args[0])->value));
}


RuntimeVal* minFunction(const std::vector<RuntimeVal*> args, Environment* env) {
  if (args.size() == 0) {
    return new NullVal();
  }

  if (args[0]->type != ValueType::NumberValue) {
    std::cerr << "Wrong argument type for min function" << std::endl;
    std::exit(1);
  }

  double minNumber = dynamic_cast<NumberVal*>(args[0])->value;

  for (size_t i = 1; i < args.size(); i++) {
    if (args[0]->type != ValueType::NumberValue) {
      std::cerr << "Wrong argument type for min function" << std::endl;
      std::exit(1);
    }

    double nextNumber = dynamic_cast<NumberVal*>(args[i])->value;
    if (nextNumber < minNumber) {
      minNumber = nextNumber;
    }
  }
  
  return new NumberVal(minNumber);
}


RuntimeVal* maxFunction(const std::vector<RuntimeVal*> args, Environment* env) {
  if (args.size() == 0) {
    return new NullVal();
  }

  if (args[0]->type != ValueType::NumberValue) {
    std::cerr << "Wrong argument type for max function" << std::endl;
    std::exit(1);
  }

  double maxNumber = dynamic_cast<NumberVal*>(args[0])->value;

  for (size_t i = 1; i < args.size(); i++) {
    if (args[0]->type != ValueType::NumberValue) {
      std::cerr << "Wrong argument type for max function" << std::endl;
      std::exit(1);
    }

    double nextNumber = dynamic_cast<NumberVal*>(args[i])->value;
    if (nextNumber > maxNumber) {
      maxNumber = nextNumber;
    }
  }
  
  return new NumberVal(maxNumber);
}

RuntimeVal* absFuntion(const std::vector<RuntimeVal*> args, Environment* env) {
  if (args.size() != 1) {
    std::cerr << "Wrong number of arguments in abs function" << std::endl;
    std::exit(1);
  }

  if (args[0]->type != ValueType::NumberValue) {
    std::cerr << "Wrong argument type for round function" << std::endl;
    std::exit(1);
  }

  double num = dynamic_cast<NumberVal*>(args[0])->value;

  if (num >= 0) {
    return args[0];
  }
  if (num < 0) {
    return new NumberVal(-num);
  }

  return new NullVal();
}



RuntimeVal* exitFunction(const std::vector<RuntimeVal*> args, Environment* env) {
    exit(1);
    return new NullVal;
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
}
