#pragma once

#ifndef VALUES_H
#define VALUES_H

#include <iostream>
#include <functional>
#include <memory>
#include "../../ast/AST.h"


enum class ValueType {
    NullValue,
    NumberValue,
    StringValue,
    BooleanValue,
    NativeFunction,
    Function,
    ReturnValue,
};


class RuntimeVal {
  public:
    ValueType type;
    virtual ~RuntimeVal() = default;
    virtual std::string toString() = 0;
    virtual std::string getType() = 0;
    RuntimeVal(ValueType type);
};


class NullVal : public RuntimeVal {
  public:
    const std::string value = "null";
    NullVal();

    std::string toString() override;
    std::string getType()  override {
        return "NullVal";
    }
};


class BooleanVal : public RuntimeVal {
  public:
    bool value;

    BooleanVal(bool b = true);
    BooleanVal(BooleanVal& orginal);

    std::string toString() override;
    std::string getType() override {
        return "BooleanVal";
    }

};


class NumberVal : public RuntimeVal {
  public:
    double value;

    NumberVal(double n = 0);
    NumberVal(const NumberVal& orginal);
    NumberVal(const NumericLiteral* numberLiteral);

    std::string toString() override;
    std::string getType() override {
        return "NumberVal";
    }
};


class ReturnValue : public RuntimeVal {
  public:
    RuntimeVal* value;

    ReturnValue(RuntimeVal* val);
    ReturnValue(ReturnValue& val);

    std::string toString() override;
    std::string getType() override {
        return "ReturnVal";
    }

    ~ReturnValue() {
        delete value;
    }
};


class StringVal : public RuntimeVal {
  public:
    std::string value;
    StringVal(const std::string& str = "");
    StringVal(const StringVal& orginal);
    StringVal(const StrLiteral* strLiteral);

    std::string toString() override;
    std::string getType() override {
        return "StringVal";
    }
};


#endif
