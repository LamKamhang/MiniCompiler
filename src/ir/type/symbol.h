#pragma once
#include "type.h"
#include <ir.h>
#include <stack>
#include <string>
namespace ir
{
class Symbol
{
protected:
    llvm::Value *value;

public:
    std::string name;
    std::shared_ptr<ir::Type> type;
    bool is_lvalue;
    std::shared_ptr<ir::Symbol> LValue();
    std::shared_ptr<ir::Symbol> RValue();
    std::shared_ptr<ir::Symbol> DeReference();
    std::shared_ptr<ir::Symbol> Reference();
    std::shared_ptr<ir::Symbol> Error(const std::string &info);
    llvm::Value *Store(llvm::Value *val);                 // unsafe assignment
    llvm::Value *Assign(std::shared_ptr<ir::Symbol> val); // type check assignment
    llvm::Value *GetValue();
    bool IsValid();
    static std::shared_ptr<ir::Symbol> Get(std::shared_ptr<ir::Type> type, const std::string &name);
    static std::shared_ptr<ir::Symbol> GetConstant(std::shared_ptr<ir::Type> type, llvm::Value *val);

    Symbol(std::shared_ptr<ir::Type> type, const std::string &name, bool is_lvalue); // normal symbol
    Symbol(std::shared_ptr<ir::Type> type);                                          // constant
protected:
};
} // namespace ir