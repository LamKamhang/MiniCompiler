#pragma once
#include <ir.h>
#include <block.h>
#include <unordered_map>
#include <../ast/ast.h>
namespace ir
{
class Generator
{
public:
    static std::unordered_map<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>, ir::Block &)>> table;
    static void init();
    static llvm::Value *generate(std::vector<std::shared_ptr<ast::Node>> &root);
    static llvm::Value *LogError(const char *str);

private:
    Generator() = default;
};
} // namespace ir