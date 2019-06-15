#pragma once
#include "ir.h"
#include "type/function.h"
#include "type/symbol.h"
#include <llvm/IR/BasicBlock.h>
#include <unordered_map>
namespace ir
{
class Block
{
public:
    std::unordered_map<std::string, std::shared_ptr<ir::Symbol>> SymbolTable;
    Block *parent = nullptr;
    Block() = default;
    Block(Block *parent) : parent(parent){};
    ir::Block *GetSymbolBlock(const std::string &name);
    std::shared_ptr<ir::Symbol> GetSymbol(const std::string &name);
    bool HasSymbol(const std::string &name);
    bool DefineSymbol(const std::string &name, std::shared_ptr<ir::Symbol> val);
    bool SetSymbol(const std::string &name, std::shared_ptr<ir::Symbol> val);
    bool HasFunction(const std::string &name);
    bool DefineFunction(std::shared_ptr<ir::FunctionTy> function, const std::string &name);
    std::shared_ptr<ir::FunctionTy> GetFunction(const std::string &name);
};
} // namespace ir