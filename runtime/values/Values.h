#pragma once


#ifndef VALUES_H
#define VALUES_H

#include <iostream>
#include <functional>
#include "../../ast/AST.h"

// Enum to represent value types
enum class ValueType {
  NullValue,
  NumberValue,
	BooleanValue,
	NativeFunction,
	Function,    
  ReturnValue,
};


class RuntimeVal {
	public:
		ValueType type ;
		virtual ~RuntimeVal() = default;
    virtual std::string toString() const {
        return "RuntimeVal";
    }
};


class NullVal : public RuntimeVal {
  public:
    const std::string value = "null";

    NullVal() {
        type = ValueType::NullValue;
    }

    std::string toString() const override {
      return value;
    }

};


class BooleanVal : public RuntimeVal {
	public:
		bool value;

    BooleanVal(bool b = true) : value(b) {
        type = ValueType::BooleanValue;
    }

    std::string toString() const override {
      return std::to_string(value);
    }

};


class NumberVal : public RuntimeVal {
	public:
		double value;

    NumberVal(double n = 0) : value(n) {
        type = ValueType::NumberValue;
    }

    std::string toString() const override {
      return std::to_string(value);
    }
};


class ReturnValue : public RuntimeVal {
  public:
    RuntimeVal* value;

    ReturnValue(RuntimeVal* val) : value(val) {
        type = ValueType::ReturnValue;
    }

    std::string toString() const override {
        return value->toString();
    }
};



#endif
