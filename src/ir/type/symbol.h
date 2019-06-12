#pragma once
#include "type.h"
#include <ir.h>
#include <stack>
#include <string>
namespace ir
{
class Symbol
{
public:
    std::string name;
    ir::Type *type;
    bool is_lvalue;
    std::shared_ptr<ir::Symbol> LValue();
    std::shared_ptr<ir::Symbol> RValue();
    static std::shared_ptr<ir::Symbol> get(ir::Type *type, const std::string &name);

protected:
    llvm::Value *value;
    Symbol(ir::Type *type, const std::string &name, bool is_lvalue);
};
} // namespace ir