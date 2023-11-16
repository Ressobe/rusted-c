#pragma once

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include "../values/Values.h"
#include <set>
#include <functional>
#include <memory>


class Environment {
	private:
		Environment* parent;
    std::unordered_map<std::string, std::unique_ptr<RuntimeVal>> variables;
		std::set<std::string> constants;

	public:
		Environment(Environment* parentEnv = nullptr);
    std::unique_ptr<RuntimeVal> declareVar(const std::string& varName, std::unique_ptr<RuntimeVal> value, bool isConst);
    std::unique_ptr<RuntimeVal> assignVar(const std::string& varName, std::unique_ptr<RuntimeVal> value);
    std::unique_ptr<RuntimeVal> lookupVar(const std::string& varName);
		Environment* resolve(const std::string& varName);

		void createGlobalEnv();
		bool isConstant(const std::string& varname);
};


typedef std::function<RuntimeVal* (const std::vector<RuntimeVal*>, Environment*)> FunctionType;

class NativeFnVal : public RuntimeVal {
	public:
		FunctionType call;
		NativeFnVal(FunctionType c);
    std::unique_ptr<RuntimeVal> clone() const override;
};

class FnVal : public RuntimeVal {
  public:
	  std::string name;
    std::vector<std::string> parameters;
    Environment* declarationEnv;

    std::vector<Stmt*> body; 

	  FnVal(std::string n, std::vector<std::string> p, Environment* d, std::vector<Stmt*> b);
    std::unique_ptr<RuntimeVal> clone() const override;
};

#endif
