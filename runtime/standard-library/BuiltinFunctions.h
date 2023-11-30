#pragma once

#ifndef BUILTIN_FUNCTIONS_H
#define BUILTIN_FUNCTIONS_H

#include "../environment/Environment.h"
#include "../values/Values.h"


void clearScreen();

RuntimeVal* printFunction(const std::vector<RuntimeVal*> args, Environment* env);
RuntimeVal* clearFunction(const std::vector<RuntimeVal*> args, Environment* env);
RuntimeVal* sqrtFunction(const std::vector<RuntimeVal*> args, Environment* env);
RuntimeVal* powFunction(const std::vector<RuntimeVal*> args, Environment* env);
RuntimeVal* roundFunction(const std::vector<RuntimeVal*> args, Environment* env);
RuntimeVal* minFunction(const std::vector<RuntimeVal*> args, Environment* env);
RuntimeVal* maxFunction(const std::vector<RuntimeVal*> args, Environment* env);
RuntimeVal* absFunction(const std::vector<RuntimeVal*> args, Environment* env);
RuntimeVal* inputFunction(const std::vector<RuntimeVal*> args, Environment* env);
RuntimeVal* numberFunction(const std::vector<RuntimeVal*> args, Environment* env);
RuntimeVal* stringFunction(const std::vector<RuntimeVal*> args, Environment* env);
RuntimeVal* lenFunction(const std::vector<RuntimeVal*> args, Environment* env);
RuntimeVal* exitFunction(const std::vector<RuntimeVal*> args, Environment* env);

#endif
