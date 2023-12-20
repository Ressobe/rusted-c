#include "BuiltinFunctions.h"

RuntimeVal *printFunction(const std::vector<RuntimeVal *> args,
                          Environment *env) {
  for (auto arg : args) {
    std::cout << arg->toString() << " ";
  }
  std::cout << std::endl;

  return new NullVal;
}

void clearScreen() {
#ifdef _WIN32
  std::system("cls");
#else
  std::system("clear");
#endif
}

std::string argumentsNumberMessage = "Wrong number of arguments in "; 

std::string argumentsTypeMessage = "Wrong argument type for";

RuntimeVal *clearFunction(const std::vector<RuntimeVal *> args,
                          Environment *env) {
  clearScreen();

  return new NullVal;
}

bool checkNumberOfArgument(int argumentsNumber, int numberToCheck) {
    if (argumentsNumber == numberToCheck) {
        return true; 
    }
    return false;
}

bool checkArgumentType(ValueType argumentToChek, ValueType typeToCheck) {
    if (argumentToChek != typeToCheck) return false;
    return true;
}

void exitWithError(const std::string& message1, const std::string& message2) {
    std::cerr << message1 <<  message2 << std::endl;
    std::exit(1);
}

RuntimeVal *sqrtFunction(const std::vector<RuntimeVal *> args,
                         Environment *env) {

  if (!checkNumberOfArgument(args.size(), 1)) {
      exitWithError(argumentsNumberMessage, "sqrt function");
  }

  if (!checkArgumentType(args[0]->type, ValueType::NumberValue)) {
      exitWithError(argumentsTypeMessage, "sqrt function");
  }

  NumberVal *number = dynamic_cast<NumberVal *>(args[0]);

  return new NumberVal(sqrt(number->value));
}

RuntimeVal *powFunction(const std::vector<RuntimeVal *> args,
                        Environment *env) {

  if (!checkNumberOfArgument(args.size(), 2)) {
      exitWithError(argumentsNumberMessage, "pow function");
  }

  if (!checkArgumentType(args[0]->type, ValueType::NumberValue) && (!checkArgumentType(args[1]->type, ValueType::NumberValue))) {
      exitWithError(argumentsTypeMessage, "pow function");
  }

  return new NumberVal(pow(dynamic_cast<NumberVal *>(args[0])->value,
                            dynamic_cast<NumberVal *>(args[1])->value));
}

RuntimeVal *roundFunction(const std::vector<RuntimeVal *> args,
                          Environment *env) {

  if (!checkNumberOfArgument(args.size(), 1)) {
      exitWithError(argumentsNumberMessage, "round function");
  }

  if (!checkArgumentType(args[0]->type, ValueType::NumberValue)) {
      exitWithError(argumentsTypeMessage, "round function");
  }

  return new NumberVal(round(dynamic_cast<NumberVal *>(args[0])->value));
}

RuntimeVal *floorFunction(const std::vector<RuntimeVal *> args,
                          Environment *env) {

  if (!checkNumberOfArgument(args.size(), 1)) {
      exitWithError(argumentsNumberMessage, "floor function");
  }

  if (!checkArgumentType(args[0]->type, ValueType::NumberValue)) {
      exitWithError(argumentsTypeMessage, "floor function");
  }

  return new NumberVal(floor(dynamic_cast<NumberVal *>(args[0])->value));
}

RuntimeVal *minFunction(const std::vector<RuntimeVal *> args,
                        Environment *env) {

  if (args.size() == 0) {
    return new NullVal();
  }

  if (!checkArgumentType(args[0]->type, ValueType::NumberValue)) {
      exitWithError(argumentsTypeMessage, "min function");
  }

  double minNumber = dynamic_cast<NumberVal *>(args[0])->value;

  for (size_t i = 1; i < args.size(); i++) {
    if (!checkArgumentType(args[i]->type, ValueType::NumberValue)) {
        exitWithError(argumentsTypeMessage, "min function");
    }

    double nextNumber = dynamic_cast<NumberVal *>(args[i])->value;
    if (nextNumber < minNumber) {
      minNumber = nextNumber;
    }
  }

  return new NumberVal(minNumber);
}

RuntimeVal *maxFunction(const std::vector<RuntimeVal *> args,
                        Environment *env) {
  if (args.size() == 0) {
    return new NullVal();
  }

  if (!checkArgumentType(args[0]->type, ValueType::NumberValue)) {
      exitWithError(argumentsTypeMessage, "max function");
  }

  double maxNumber = dynamic_cast<NumberVal *>(args[0])->value;

  for (size_t i = 1; i < args.size(); i++) {
    if (!checkArgumentType(args[i]->type, ValueType::NumberValue)) {
        exitWithError(argumentsTypeMessage, "max function");
    }

    double nextNumber = dynamic_cast<NumberVal *>(args[i])->value;
    if (nextNumber > maxNumber) {
      maxNumber = nextNumber;
    }
  }

  return new NumberVal(maxNumber);
}

RuntimeVal *absFunction(const std::vector<RuntimeVal *> args,
                        Environment *env) {

  if (!checkNumberOfArgument(args.size(), 1)) {
      exitWithError(argumentsNumberMessage, "abs function");
  }

  if (!checkArgumentType(args[0]->type, ValueType::NumberValue)) {
      exitWithError(argumentsTypeMessage, "abs funcion");
  }

  double num = dynamic_cast<NumberVal *>(args[0])->value;

  if (num >= 0) {
    return args[0];
  }
  if (num < 0) {
    return new NumberVal(-num);
  }

  return new NullVal();
}

RuntimeVal *inputFunction(const std::vector<RuntimeVal *> args,
                          Environment *env) {
  if (args.size() > 1) {
    std::cerr
        << "Wrong number of arguments in input function, expected only one"
        << std::endl;
    std::exit(1);
  }

  if (args.size() == 1) {
    if (!checkArgumentType(args[0]->type, ValueType::StringValue)) {
        exitWithError(argumentsTypeMessage, "input function");
    }
    std::cout << dynamic_cast<StringVal *>(args[0])->value << std::endl;
  }

  std::string input;
  std::cin >> input;

  return new StringVal(input);
}

RuntimeVal *numberFunction(const std::vector<RuntimeVal *> args,
                           Environment *env) {

  if (!checkNumberOfArgument(args.size(), 1)) {
      exitWithError(argumentsNumberMessage, "num function");
  }

  if (!checkArgumentType(args[0]->type, ValueType::StringValue)) {
      exitWithError(argumentsTypeMessage, "num function");
  }

  return new NumberVal(std::stoi(dynamic_cast<StringVal *>(args[0])->value));
}

RuntimeVal *stringFunction(const std::vector<RuntimeVal *> args,
                           Environment *env) {

  if (!checkNumberOfArgument(args.size(), 1)) {
      exitWithError(argumentsNumberMessage, "str function");
  }

  if (!checkArgumentType(args[0]->type, ValueType::NumberValue)) {
      exitWithError(argumentsTypeMessage, "str function");
  }

  return new StringVal(
      std::to_string(dynamic_cast<NumberVal *>(args[0])->value));
}

RuntimeVal *lenFunction(const std::vector<RuntimeVal *> args,
                        Environment *env) {

  if (!checkNumberOfArgument(args.size(), 1)) {
      exitWithError(argumentsNumberMessage, "len function");
  }

  if (!checkArgumentType(args[0]->type, ValueType::StringValue)) {
      exitWithError(argumentsTypeMessage, "len function");
  }

  StringVal *stringValue = dynamic_cast<StringVal *>(args[0]);

  int strLen = std::strlen(stringValue->value.c_str());

  NumberVal *stringLenght = new NumberVal(strLen);

  return stringLenght;
}

RuntimeVal *exitFunction(const std::vector<RuntimeVal *> args,
                         Environment *env) {
  exit(1);
  return new NullVal;
}

RuntimeVal *typeFunction(const std::vector<RuntimeVal *> args,
                         Environment *env) {
  if (!checkNumberOfArgument(args.size(), 1)) {
      exitWithError(argumentsNumberMessage, "type function");
  }

  return new StringVal(args[0]->getType());
}

RuntimeVal *concatFunction(const std::vector<RuntimeVal *> args,
                            Environment *env) {
  std::string result;

  for (auto arg : args) {
    if (!checkArgumentType(arg->type, ValueType::StringValue)) {
      exitWithError(argumentsTypeMessage, "concat function");
    }

    result += dynamic_cast<StringVal *>(arg)->value;
  }

  return new StringVal(result);
}

RuntimeVal *sinFunction(const std::vector<RuntimeVal *> args,
                        Environment *env) {
  if (!checkNumberOfArgument(args.size(), 1)) {
      exitWithError(argumentsNumberMessage, "sin function");
  }

  if (!checkArgumentType(args[0]->type, ValueType::NumberValue)) {
      exitWithError(argumentsTypeMessage, "sin function");
  }

  return new NumberVal(std::sin(dynamic_cast<NumberVal *>(args[0])->value));
}

RuntimeVal *cosFunction(const std::vector<RuntimeVal *> args,
                        Environment *env) {
  if (!checkNumberOfArgument(args.size(), 1)) {
      exitWithError(argumentsNumberMessage, "cos function");
  }

  if (!checkArgumentType(args[0]->type, ValueType::NumberValue)) {
      exitWithError(argumentsTypeMessage, "cos function");
  }

  return new NumberVal(std::cos(dynamic_cast<NumberVal *>(args[0])->value));
}

RuntimeVal *tanFunction(const std::vector<RuntimeVal *> args,
                        Environment *env) {
  if (!checkNumberOfArgument(args.size(), 1)) {
      exitWithError(argumentsNumberMessage, "tan function");
  }

  if (!checkArgumentType(args[0]->type, ValueType::NumberValue)) {
      exitWithError(argumentsTypeMessage, "tan function");
  }

  return new NumberVal(std::tan(dynamic_cast<NumberVal *>(args[0])->value));
}

RuntimeVal *logFunction(const std::vector<RuntimeVal *> args,
                        Environment *env) {
  if (!checkNumberOfArgument(args.size(), 1)) {
      exitWithError(argumentsNumberMessage, "log function");
  }

  if (!checkArgumentType(args[0]->type, ValueType::NumberValue)) {
      exitWithError(argumentsTypeMessage, "log function");
  }

  return new NumberVal(std::log(dynamic_cast<NumberVal *>(args[0])->value));
}

RuntimeVal *ceilFunction(const std::vector<RuntimeVal *> args,
                         Environment *env) {
  if (!checkNumberOfArgument(args.size(), 1)) {
      exitWithError(argumentsNumberMessage, "ceil function");
  }

  if (!checkArgumentType(args[0]->type, ValueType::NumberValue)) {
      exitWithError(argumentsTypeMessage, "ceil function");
  }

  return new NumberVal(std::ceil(dynamic_cast<NumberVal *>(args[0])->value));
}
