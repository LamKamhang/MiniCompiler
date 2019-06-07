#pragma once
#include "ir.h"
#include "type.h"
#include <unordered_map>
#include <llvm/IR/BasicBlock.h>
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
    bool setSymbol(const std::string &name, llvm::Value *val);
    llvm::Type *getCustomType(const std::string &type);
};
} // namespace ir