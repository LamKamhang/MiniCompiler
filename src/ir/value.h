#pragma once
#include "ir.h"
#include "type.h"
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>

namespace ir
{
class CustomValue : public llvm::Value
{
public:
    ir::Type *type;
    int pos;
    CustomValue() = default;
    void setType(ir::Type *type);
    llvm::Value deRef();
};
} // namespace ir