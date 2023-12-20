#ifndef BUILTIN_FUNCTIONS_H
#define BUILTIN_FUNCTIONS_H

#include "../environment/Environment.h"
#include "../values/Values.h"
#include <iostream>
#include <format>

void clearScreen();

bool checkNumberOfArgument(int argumentsNumber, int numberToChekc);

bool checkArgumentType(ValueType argumentToChek, ValueType typeToCheck);

void exitWithError(std::string message);

RuntimeVal *printFunction(const std::vector<RuntimeVal *> args,
                          Environment *env);
RuntimeVal *clearFunction(const std::vector<RuntimeVal *> args,
                          Environment *env);
RuntimeVal *sqrtFunction(const std::vector<RuntimeVal *> args,
                         Environment *env);
RuntimeVal *powFunction(const std::vector<RuntimeVal *> args, Environment *env);
RuntimeVal *roundFunction(const std::vector<RuntimeVal *> args,
                          Environment *env);
RuntimeVal *floorFunction(const std::vector<RuntimeVal *> args,
                          Environment *env);
RuntimeVal *minFunction(const std::vector<RuntimeVal *> args, Environment *env);
RuntimeVal *maxFunction(const std::vector<RuntimeVal *> args, Environment *env);
RuntimeVal *absFunction(const std::vector<RuntimeVal *> args, Environment *env);
RuntimeVal *inputFunction(const std::vector<RuntimeVal *> args,
                          Environment *env);
RuntimeVal *numberFunction(const std::vector<RuntimeVal *> args,
                           Environment *env);
RuntimeVal *stringFunction(const std::vector<RuntimeVal *> args,
                           Environment *env);
RuntimeVal *lenFunction(const std::vector<RuntimeVal *> args, Environment *env);
RuntimeVal *exitFunction(const std::vector<RuntimeVal *> args,
                         Environment *env);

RuntimeVal *typeFunction(const std::vector<RuntimeVal *> args,
                         Environment *env);

RuntimeVal *concatFunction(const std::vector<RuntimeVal *> args,
                         Environment *env);

RuntimeVal *sinFunction(const std::vector<RuntimeVal *> args,
                         Environment *env);

RuntimeVal *cosFunction(const std::vector<RuntimeVal *> args,
                         Environment *env);

RuntimeVal *tanFunction(const std::vector<RuntimeVal *> args,
                         Environment *env);

RuntimeVal *logFunction(const std::vector<RuntimeVal *> args,
                         Environment *env);

RuntimeVal *ceilFunction(const std::vector<RuntimeVal *> args,
                         Environment *env);
#endif
