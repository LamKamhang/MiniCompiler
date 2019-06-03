#pragma once
#include <ir.h>
#include <llvm/IR/Function.h>
namespace ir
{
class Function : public llvm::Function
{
};
} // namespace ir