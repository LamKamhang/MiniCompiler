#pragma once
#include "ir.h"
#include "type/type.h"
#include <llvm/IR/BasicBlock.h>
#include <unordered_map>
namespace ir
{
class Block
{
public:
    std::unordered_map<std::string, llvm::Value *> SymbolTable;
    Block *parent = nullptr;
    Block() = default;
    Block(Block *parent) : parent(parent){};
    llvm::Value *getSymbol(const std::string &name);
    Block *getSymbolTable(const std::string &name);
    bool defineSymbol(const std::string &name, llvm::Value *val);
    bool setSymbol(const std::string &name, llvm::Value *val);
};
} // namespace ir