#include "Values.h" 


NullVal::NullVal() {
    type = ValueType::NullValue;
}


NullVal* NullVal::MK_NULL() {
    return new NullVal();
}

BooleanVal::BooleanVal(bool b) : value(b) {
    type = ValueType::BooleanValue;
}

BooleanVal* BooleanVal::MK_BOOL(bool b) {
    return new BooleanVal(b);
}

NumberVal::NumberVal(double n) : value(n) {
    type = ValueType::NumberValue;
}


NumberVal* NumberVal::MK_NUMBER(double n) {
    return new NumberVal(n);
}
