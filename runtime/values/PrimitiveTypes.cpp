#include "Values.h"

RuntimeVal::RuntimeVal(ValueType type) : type(type){};

NullVal::NullVal() : RuntimeVal(ValueType::NullValue) {}

std::string NullVal::toString() { return value; }

BooleanVal::BooleanVal(bool b)
    : RuntimeVal(ValueType::BooleanValue), value(b) {}

BooleanVal::BooleanVal(BooleanVal &orginal)
    : RuntimeVal(ValueType::BooleanValue), value(orginal.value) {}

std::string BooleanVal::toString() {
  if (value) {
    return "true";
  }
  return "false";
}

NumberVal::NumberVal(double n) : RuntimeVal(ValueType::NumberValue), value(n) {}

NumberVal::NumberVal(const NumberVal &orginal)
    : RuntimeVal(ValueType::NumberValue), value(orginal.value) {}

NumberVal::NumberVal(const NumericLiteral *numberLiteral)
    : RuntimeVal(ValueType::NumberValue), value(numberLiteral->value) {}

NumberVal* NumberVal::operator+(const NumberVal& other) const {
    return new NumberVal(this->value + other.value);
}

NumberVal* NumberVal::operator-(const NumberVal& other) const {
    return new NumberVal(this->value - other.value);
}

NumberVal* NumberVal::operator*(const NumberVal& other) const {
    return new NumberVal(this->value * other.value);
}

NumberVal* NumberVal::operator/(const NumberVal& other) const {
    return new NumberVal(this->value / other.value);
}

NumberVal* NumberVal::operator<(const NumberVal& other) const {
    return new NumberVal(this->value < other.value);
}

NumberVal* NumberVal::operator<=(const NumberVal& other) const {
    return new NumberVal(this->value <= other.value);
}

NumberVal* NumberVal::operator>=(const NumberVal& other) const {
    return new NumberVal(this->value >= other.value);
}

NumberVal* NumberVal::operator>(const NumberVal& other) const {
    return new NumberVal(this->value > other.value);
}

NumberVal* NumberVal::operator==(const NumberVal& other) const {
    return new NumberVal(this->value == other.value);
}

NumberVal* NumberVal::operator!=(const NumberVal& other) const {
    return new NumberVal(this->value != other.value);
}

NumberVal* NumberVal::operator%(const NumberVal& other) const {
    return new NumberVal(fmod(this->value, other.value));
}

NumberVal* NumberVal::operator&&(const NumberVal& other) const {
    return new NumberVal(this->value && other.value);
}

NumberVal* NumberVal::operator||(const NumberVal& other) const {
    return new NumberVal(this->value || other.value);
}

std::string NumberVal::toString() {
  int valueInt = value;
  if ((value - valueInt) == 0) {
    return std::to_string(valueInt);
  }
  return std::to_string(value);
}

ReturnValue::ReturnValue(RuntimeVal *val)
    : RuntimeVal(ValueType::ReturnValue), value(val) {}

ReturnValue::ReturnValue(ReturnValue &orginal)
    : RuntimeVal(ValueType::ReturnValue), value(orginal.value) {}

std::string ReturnValue::toString() { return value->toString(); }

StringVal::StringVal(const std::string &str)
    : RuntimeVal(ValueType::StringValue), value(str) {}

StringVal::StringVal(const StringVal &orginal)
    : RuntimeVal(ValueType::StringValue), value(orginal.value) {}

StringVal::StringVal(const StrLiteral *strLiteral)
    : RuntimeVal(ValueType::StringValue), value(strLiteral->value) {}

std::string StringVal::toString() { return value; }
