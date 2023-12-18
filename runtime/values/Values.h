#ifndef VALUES_H
#define VALUES_H

#include <iostream>
#include <functional>
#include <memory>
#include <map>

class Environment; // Deklaracja wstêpna

#include "../../ast/AST.h"


enum class ValueType {
    NullValue,
    NumberValue,
    StringValue,
    BooleanValue,

    NativeFunction,
    Function,
    ReturnValue,
    StructValue,
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

typedef std::function<RuntimeVal* (const std::vector<RuntimeVal*>, Environment*)> FunctionType;

class NativeFnVal : public RuntimeVal {
	public:
		FunctionType call;
		NativeFnVal(FunctionType c);

    std::string toString() override {
      return "NativeFnVal";
    }

    std::string getType() override {
      return "NativeFnVal";
    }
};


class FnVal : public RuntimeVal {
	public:
 		std::string name;
		std::vector<std::string> parameters;
		Environment* declarationEnv;
		std::vector<Stmt*> body;

		FnVal(std::string n, std::vector<std::string> p, Environment* d, std::vector<Stmt*> b);

    std::string toString() override {
      return "FnVal";
    }

    std::string getType() override {
      return "FnVal";
    }
 };


class StructVal : public RuntimeVal {
  public:
    bool isDeclaration;
    std::string structName;
    std::map<std::string, RuntimeVal*> fields;


  public:
    StructVal(const std::string& name, bool isDecl);

    RuntimeVal* getField(const std::string& fieldName);
    void setField(const std::string& fieldName, RuntimeVal* value);
    void addField(const std::string& fieldName, RuntimeVal* value);

    std::string toString() override;
    std::string getType() override;
};


#endif
