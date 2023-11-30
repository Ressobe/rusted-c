#include "Values.h"

// NullVal
NullVal::NullVal() {
    type = ValueType::NullValue;
}

std::string NullVal::toString() const {
    return value;
}

std::unique_ptr<RuntimeVal> NullVal::clone() const {
    return std::make_unique<NullVal>(*this);
}

// BooleanVal
BooleanVal::BooleanVal(bool b) : value(b) {
    type = ValueType::BooleanValue;
}

std::string BooleanVal::toString() const {
    if (value) {
        return "true";
    }
    return "false";
}

std::unique_ptr<RuntimeVal> BooleanVal::clone() const {
    return std::make_unique<BooleanVal>(*this);
}

// NumberVal
NumberVal::NumberVal(double n) : value(n) {
    type = ValueType::NumberValue;
}

std::string NumberVal::toString() const {
    int valueInt = value;
    if ((value - valueInt) == 0) {
      return std::to_string(valueInt);
    }
    return std::to_string(value);
}

std::unique_ptr<RuntimeVal> NumberVal::clone() const {
    return std::make_unique<NumberVal>(*this);
}

// ReturnValue
ReturnValue::ReturnValue(RuntimeVal* val) : value(val) {
    type = ValueType::ReturnValue;
}

std::string ReturnValue::toString() const {
    return value->toString();
}

std::unique_ptr<RuntimeVal> ReturnValue::clone() const {
    return std::make_unique<ReturnValue>(*this);
}

StringVal::StringVal(const std::string& str) : value(str) {
    type = ValueType::StringValue;
}

std::string StringVal::toString() const {
    return value;
}

std::unique_ptr<RuntimeVal> StringVal::clone() const {
    return std::make_unique<StringVal>(*this);
}
