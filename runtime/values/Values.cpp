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
    type = ValueType::BooleanValue;
}

std::string BooleanVal::toString() const {
    if (value) {
        return "true";
    }
    return "false";
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


// ReturnValue
ReturnValue::ReturnValue(RuntimeVal* val) : value(val) {
    type = ValueType::ReturnValue;
}

std::string ReturnValue::toString() const {
    return value->toString();
}


StringVal::StringVal(const std::string& str) : value(str) {
    type = ValueType::StringValue;
}

std::string StringVal::toString() const {
    return value;
}
