#include <map>
#include <string>
#include <functional>
#include <memory>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include "../lib/json/json.h"

namespace ir
{
static std::unique_ptr<llvm::LLVMContext> Context;
static std::unique_ptr<llvm::IRBuilder<>> Builder;
static std::unique_ptr<llvm::Module> Module;
static std::unique_ptr<std::map<std::string, ir::Type>> TypeMap;
void createModule();
} // namespace ir