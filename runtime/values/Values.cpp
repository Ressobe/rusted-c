#include "Values.h"


RuntimeVal::RuntimeVal(ValueType type) : type(type) {};


NullVal::NullVal() : RuntimeVal(ValueType::NullValue) {}


std::string NullVal::toString() {
    return value;
}


BooleanVal::BooleanVal(bool b) : RuntimeVal(ValueType::BooleanValue), value(b) {}


BooleanVal::BooleanVal(BooleanVal& orginal) : RuntimeVal(ValueType::BooleanValue), value(orginal.value) {}


std::string BooleanVal::toString() {
    if (value) {
        return "true";
    }
    return "false";
}


NumberVal::NumberVal(double n) : RuntimeVal(ValueType::NumberValue), value(n) {}


NumberVal::NumberVal(const NumberVal& orginal) : RuntimeVal(ValueType::NumberValue), value(orginal.value) {}


NumberVal::NumberVal(const NumericLiteral* numberLiteral) : RuntimeVal(ValueType::NumberValue), value(numberLiteral->value) {}


std::string NumberVal::toString() {
    int valueInt = value;
    if ((value - valueInt) == 0) {
      return std::to_string(valueInt);
    }
    return std::to_string(value);
}


ReturnValue::ReturnValue(RuntimeVal* val) : RuntimeVal(ValueType::ReturnValue), value(val) {}


ReturnValue::ReturnValue(ReturnValue& orginal) : RuntimeVal(ValueType::ReturnValue), value(orginal.value) {}


std::string ReturnValue::toString() {
    return value->toString();
}


StringVal::StringVal(const std::string& str) : RuntimeVal(ValueType::StringValue), value(str) {}


StringVal::StringVal(const StringVal& orginal) : RuntimeVal(ValueType::StringValue), value(orginal.value) {}


StringVal::StringVal(const StrLiteral* strLiteral) : RuntimeVal(ValueType::StringValue), value(strLiteral->value) {}


std::string StringVal::toString() {
    return value;
}

NativeFnVal::NativeFnVal(FunctionType c) : RuntimeVal(ValueType::NativeFunction), call(c) {}


FnVal::FnVal(std::string n, std::vector<std::string> p, Environment* d, std::vector<Stmt*> b)
    : RuntimeVal(ValueType::Function), name(n), parameters(std::move(p)), declarationEnv(d), body(std::move(b)) {}


StructVal::StructVal(const std::string& name, bool isDecl) : RuntimeVal(ValueType::StructValue), structName(name), isDeclaration(isDecl) {}

RuntimeVal* StructVal::getField(const std::string& fieldName) {
    // Sprawdzanie, czy pole istnieje w klasie obiektu
    auto it = this->fields.find(fieldName);

    if (it != this->fields.end()) {
        // Jeœli pole istnieje, zwracamy jego wartoœæ
        return it->second;
    } else {
        // Jeœli pole nie istnieje, zg³aszamy b³¹d
        std::cerr << "Error: Field '" << fieldName << "' not found in struct '" << this->structName << "'" << std::endl;
        std::exit(1);
    }
    return nullptr;
}


void StructVal::addField(const std::string& fieldName, RuntimeVal* value) {
    this->fields.insert({ fieldName, value });
}

std::string StructVal::toString() {
    std::string result = "Struct " + structName + " {";

    for (const auto& field : fields) {
        result += "\n  " + field.first + ": " + field.second->toString();
    }

    result += "\n}";

    return result;    

}

std::string StructVal::getType() {
    return "Object";
}

void StructVal::setField(const std::string& fieldName, RuntimeVal* value) {
    fields[fieldName] = value;
}

