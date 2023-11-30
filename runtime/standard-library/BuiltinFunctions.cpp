#include "BuiltinFunctions.h"


RuntimeVal* printFunction(const std::vector<RuntimeVal*> args, Environment* env) {
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


RuntimeVal* inputFunction(const std::vector<RuntimeVal*> args, Environment* env) {
  if (args.size() > 1 ) {
    std::cerr << "Wrong number of arguments in input function, expected only one" << std::endl;
    std::exit(1);
  }

  if (args.size() == 1) {
    if (args[0]->type != ValueType::StringValue) {
      std::cerr << "Wrong type of argument in input function, expected string value" << std::endl;
      std::exit(1);
    }
    std::cout << dynamic_cast<StringVal*>(args[0])->value << std::endl;
  }


  std::string input;
  std::cin >> input;


  return new StringVal(input);
}


RuntimeVal* numberFunction(const std::vector<RuntimeVal*> args, Environment* env) {
  if (args.size() != 1) {
    std::cerr << "Wrong number of arguments in num function, expected only one" << std::endl;
    std::exit(1);
  }

  if (args[0]->type != ValueType::StringValue) {
      std::cerr << "Wrong type of argument in num function, expected string value" << std::endl;
      std::exit(1);
  }

  return new NumberVal(std::stoi(dynamic_cast<StringVal*>(args[0])->value));
}


RuntimeVal* stringFunction(const std::vector<RuntimeVal*> args, Environment* env) {
  if (args.size() != 1) {
    std::cerr << "Wrong number of arguments in str function, expected only one" << std::endl;
    std::exit(1);
  }

  if (args[0]->type != ValueType::NumberValue) {
      std::cerr << "Wrong type of argument in str function, expected number value" << std::endl;
      std::exit(1);
  }

  return new StringVal(std::to_string(dynamic_cast<NumberVal*>(args[0])->value));
}


RuntimeVal* lenFunction(const std::vector<RuntimeVal*> args, Environment* env) {
  if (args.size() != 1) {
    std::cerr << "Wrong number of arguments in len function, expected only one" << std::endl;
    std::exit(1);
  }

  if (args[0]->type != ValueType::StringValue) {
      std::cerr << "Wrong type of argument in str function, expected number value" << std::endl;
      std::exit(1);
  }

  StringVal* stringValue = dynamic_cast<StringVal*>(args[0]);

  int strLen = std::strlen(stringValue->value.c_str());

  NumberVal* stringLenght = new NumberVal(strLen);

  return stringLenght;
}


RuntimeVal* exitFunction(const std::vector<RuntimeVal*> args, Environment* env) {
    exit(1);
    return new NullVal;
}
