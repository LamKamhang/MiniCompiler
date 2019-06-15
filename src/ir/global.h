#pragma once
#include "generator.h"
#include "ir.h"

extern ir::Generator generator;
extern std::unordered_map<std::string, std::shared_ptr<ir::FunctionTy>> FunctionTable;
