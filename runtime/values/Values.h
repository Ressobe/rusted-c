#pragma once

#ifndef VALUES_H
#define VALUES_H

#include <iostream>
#include <functional>
#include <memory>
#include "../../ast/AST.h"

// Enum to represent value types
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
    virtual std::string toString() const {
        return "RuntimeVal";
    }

    virtual std::string getType() const {
        return "RuntimeVal";
    }
};


class NullVal : public RuntimeVal {
  public:
    const std::string value = "null";
    NullVal();

    std::string toString() const override;
    std::string getType() const override {
        return "NullVal";
    }
};


class BooleanVal : public RuntimeVal {
  public:
    bool value;

    BooleanVal(bool b = true);
    BooleanVal(BooleanVal& orginal);

    std::string toString() const override;
    std::string getType() const override {
        return "BooleanVal";
    }

};


class NumberVal : public RuntimeVal {
  public:
    double value;

    NumberVal(double n = 0);
    NumberVal(const NumberVal& orginal);
    NumberVal(const NumericLiteral* numberLiteral);

    std::string toString() const override;
    std::string getType() const override {
        return "NumberVal";
    }
};


class ReturnValue : public RuntimeVal {
  public:

    RuntimeVal* value;

    ReturnValue(RuntimeVal* val);
    ReturnValue(ReturnValue& val);

    std::string toString() const override;
    std::string getType() const override {
        return "ReturnVal";
    }
};


class StringVal : public RuntimeVal {
  public:
    std::string value;
    StringVal(const std::string& str = "");
    StringVal(const StringVal& orginal);
    StringVal(const StrLiteral* strLiteral);

    std::string toString() const override;
    std::string getType() const override {
        return "StringVal";
    }
};


#endif
