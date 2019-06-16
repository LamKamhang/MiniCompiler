#pragma once
#include "../ast/ast.h"
#include "generator.h"
#include "ir.h"
#include "string"

extern ir::Generator generator;
extern std::unordered_map<std::string, std::shared_ptr<ir::FunctionTy>> FunctionTable;
extern ast::Node* current_node;
extern void Warning(ast::Node *node, const std::string &info);
extern void Errors(ast::Node *node, const std::string &info) throw(const char *);