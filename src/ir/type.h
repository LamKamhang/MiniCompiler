#pragma once
#include "ir.h"
#include <llvm/IR/Type.h>
#include <stack>
#include <string>
#include <map>
namespace ir
{
class Type
{
public:
    llvm::Type* lty;
    bool constant;
    Type(llvm::Type* type, bool constant = false): lty(type),constant(constant){};
};

} // namespace ir