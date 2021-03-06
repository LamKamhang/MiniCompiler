#pragma once
#include "../lib/json/json.h"
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <memory>

// globals
extern std::unique_ptr<llvm::LLVMContext> context;
extern std::unique_ptr<llvm::IRBuilder<>> builder;
extern std::unique_ptr<llvm::Module> module;
namespace ir
{
void CreateIrUnit();
} // namespace ir