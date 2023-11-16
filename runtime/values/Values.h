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
    virtual std::unique_ptr<RuntimeVal> clone() const = 0;
};

class NullVal : public RuntimeVal {
public:
    const std::string value = "null";

    NullVal();

    std::string toString() const override;

    std::unique_ptr<RuntimeVal> clone() const override;
};

class BooleanVal : public RuntimeVal {
public:
    bool value;

    BooleanVal(bool b = true);

    std::string toString() const override;

    std::unique_ptr<RuntimeVal> clone() const override;
};

class NumberVal : public RuntimeVal {
public:
    double value;

    NumberVal(double n = 0);

    std::string toString() const override;

    std::unique_ptr<RuntimeVal> clone() const override;
};

class ReturnValue : public RuntimeVal {
public:
    RuntimeVal* value;

    ReturnValue(RuntimeVal* val);

    std::string toString() const override;

    std::unique_ptr<RuntimeVal> clone() const override;
};

#endif
