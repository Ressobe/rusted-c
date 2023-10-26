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
};


class RuntimeVal {
	public:
		ValueType type ;
		virtual ~RuntimeVal() = default;
};


class NullVal : public RuntimeVal {
public:
	NullVal();
	static NullVal* MK_NULL();
	const std::string value = "null";
};


class BooleanVal : public RuntimeVal {
	public:
		bool value;
		BooleanVal(bool b = true);
		static BooleanVal* MK_BOOL(bool b = true);
};

class NumberVal : public RuntimeVal {
	public:
		double value;
		NumberVal(double n = 0);
		static NumberVal* MK_NUMBER(double n = 0);
};
#endif
