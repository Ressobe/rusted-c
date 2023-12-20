#include "Values.h"

NativeFnVal::NativeFnVal(FunctionType c)
    : RuntimeVal(ValueType::NativeFunction), call(c) {}

FnVal::FnVal(std::string n, std::vector<std::string> p, Environment *d,
             std::vector<Stmt *> b)
    : RuntimeVal(ValueType::Function), name(n), parameters(std::move(p)),
      declarationEnv(d), body(std::move(b)) {}

StructVal::StructVal(const std::string &name, bool isDecl)
    : RuntimeVal(ValueType::StructValue), structName(name),
      isDeclaration(isDecl) {}

RuntimeVal *StructVal::getField(const std::string &fieldName) {
  auto it = this->fields.find(fieldName);

  if (it != this->fields.end()) {
    return it->second;
  } else {
    std::cerr << "Error: Field '" << fieldName << "' not found in struct '"
              << this->structName << "'" << std::endl;
    std::exit(1);
  }
  return nullptr;
}

void StructVal::addField(const std::string &fieldName, RuntimeVal *value) {
  this->fields.insert({fieldName, value});
}

std::string StructVal::toString() {
  std::string result = "Struct " + structName + " {";

  for (const auto &field : fields) {
    result += "\n  " + field.first + ": " + field.second->toString();
  }

  result += "\n}";

  return result;
}

std::string StructVal::getType() { 
  if (this->isDeclaration) {
    return "Struct declaration"; 
  }
  return "Struct instance";
}

void StructVal::setField(const std::string &fieldName, RuntimeVal *value) {
  fields[fieldName] = value;
}
