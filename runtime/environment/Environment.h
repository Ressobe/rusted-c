#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <cstdlib>
#include <cstring>
#include <functional>
#include <map>
#include <math.h>
#include <set>
#include <string>
#include <unordered_map>

class RuntimeVal;
class NativeFnVal;
class FnVal;
class StructVal;
class InterpreterError;

class Environment {
private:
  Environment *parent;
  std::unordered_map<std::string, RuntimeVal *> variables;
  std::set<std::string> constants;

public:
  Environment(Environment *parentEnv = nullptr);
  RuntimeVal *declareVar(const std::string &varName, RuntimeVal *value,
                         bool isConst);
  RuntimeVal *assignVar(const std::string &varName, RuntimeVal *value);
  RuntimeVal *lookupVar(const std::string &varName);
  Environment *resolve(const std::string &varName);
  void createGlobalEnv();
  void createBuilinFunctions();
  bool isConstant(const std::string &varname);

  ~Environment() { delete parent; }
};

#endif
