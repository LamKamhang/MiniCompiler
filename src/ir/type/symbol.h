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
    llvm::Value *value;
    std::shared_ptr<ir::Symbol> LValue();
    std::shared_ptr<ir::Symbol> RValue();
    std::shared_ptr<ir::Symbol> DeReference();
    std::shared_ptr<ir::Symbol> Error(const std::string &info);
    llvm::Value *Store(llvm::Value *val);
    bool IsValid();
    static std::shared_ptr<ir::Symbol> Get(ir::Type *type, const std::string &name);
    static std::shared_ptr<ir::Symbol> GetConstant(ir::Type *type, llvm::Value *val);

    Symbol(ir::Type *type, const std::string &name, bool is_lvalue); // normal symbol
protected:
    Symbol(ir::Type *type); // constant
};
} // namespace ir