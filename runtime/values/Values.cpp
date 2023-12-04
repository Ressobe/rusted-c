#include "Values.h"

// NullVal
NullVal::NullVal() {
    type = ValueType::NullValue;
}

std::string NullVal::toString() const {
    return value;
}

// BooleanVal
BooleanVal::BooleanVal(bool b) : value(b) {
    this->type = ValueType::BooleanValue;
}

BooleanVal::BooleanVal(BooleanVal& orginal) {
  this->type = ValueType::BooleanValue;
  this->value = orginal.value;
}

std::string BooleanVal::toString() const {
    if (value) {
        return "true";
    }
    return "false";
}


// NumberVal
NumberVal::NumberVal(double n) : value(n) {
    this->type = ValueType::NumberValue;
}

NumberVal::NumberVal(const NumberVal& orginal) {
    this->type = ValueType::NumberValue;
    this->value = orginal.value;
}

NumberVal::NumberVal(const NumericLiteral* numberLiteral) {
    this->type = ValueType::NumberValue;
    this->value = numberLiteral->value; 
}

std::string NumberVal::toString() const {
    int valueInt = value;
    if ((value - valueInt) == 0) {
      return std::to_string(valueInt);
    }
    return std::to_string(value);
}


// ReturnValue
ReturnValue::ReturnValue(RuntimeVal* val) : value(val) {
    type = ValueType::ReturnValue;
}

ReturnValue::ReturnValue(ReturnValue& orginal) {
    this->type = ValueType::ReturnValue;
    this->value = orginal.value;
}

std::string ReturnValue::toString() const {
    return value->toString();
}


StringVal::StringVal(const std::string& str) : value(str) {
    type = ValueType::StringValue;
}


StringVal::StringVal(const StringVal& orginal) {
    this->type = ValueType::StringValue;
    this->value = orginal.value;
}


StringVal::StringVal(const StrLiteral* strLiteral) {
    this->type = ValueType::StringValue;
    this->value = strLiteral->value;
}

std::string StringVal::toString() const {
    return value;
}
