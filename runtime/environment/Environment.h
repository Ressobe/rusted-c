#pragma once

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include "../values/Values.h"
#include <set>
#include <functional>
#include <cstdlib>
#include <math.h>
#include <cstring>


class Environment {
	private:
		Environment* parent;
		std::unordered_map<std::string, RuntimeVal*> variables;
		std::set<std::string> constants;

	public:
		Environment(Environment* parentEnv = nullptr);
		RuntimeVal* declareVar(const std::string& varName, RuntimeVal* value, bool isConst);
		RuntimeVal* assignVar(const std::string& varName, RuntimeVal* value);
		RuntimeVal* lookupVar(const std::string& varName);
		Environment* resolve(const std::string& varName);
		void createGlobalEnv();
    void createBuilinFunctions();
		bool isConstant(const std::string& varname);

    ~Environment() {
      delete parent;
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

#endif
