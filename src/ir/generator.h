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
    Generator() { this->Init(); };
};
} // namespace ir
