#pragma once
#include "../ast/ast.h"
#include "block.h"
#include "ir.h"
#include "type/symbol.h"
#include <functional>
#include <map>
namespace ir
{
class Generator
{
private:
    std::map<std::string, std::function<bool(std::shared_ptr<ast::Node>, ir::Block &)>> generate_code;
    std::map<std::string, std::function<std::shared_ptr<ir::Symbol>(std::shared_ptr<ast::Node>, ir::Block &)>> resolve_symbol;

public:
    void Init();
    bool Generate(std::shared_ptr<ast::Node> &object);
    bool Error(ast::Node *node, const std::string &str);
    Generator() { this->Init(); };
};
} // namespace ir
extern ir::Generator generator;