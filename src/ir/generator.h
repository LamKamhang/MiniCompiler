#pragma once
#include <../ast/ast.h>
#include <ir.h>
#include <block.h>
#include <functional>
#include <map>
namespace ir
{
class Generator
{
public:
    std::map<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>, ir::Block &)>> table;
    void init();
    llvm::Value *generate(std::vector<std::shared_ptr<ast::Node>> &objects);
    llvm::Value *LogError(const char *str);
    Generator() { this->init(); };
};
} // namespace ir
