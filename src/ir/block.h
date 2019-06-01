#pragma once
#include <ir.h>
#include <type.h>
#include <unordered_map>
#include <llvm/IR/BasicBlock.h>
namespace ir
{
class Block
{
public:
    std::unordered_map<std::string, llvm::Type> TypeTable;
    std::unordered_map<std::string, llvm::Value *> SymbolTable;
    Block *parent;
    llvm::BasicBlock *bb;
    Block() = default;
    template <typename... T>
    static const ir::Type &getCustomType(const ir::Block &block, T... args)
    {
        const ir::Type type;
        const ir::Type &res = type;
        type.typeStack.push(ir::Type::getInt32Ty(ir::Context));
        block.TypeTable.insert({"int", std::move(type)});
        auto &context = ir::Context;
        auto &typeMap = ir::TypeMap;
        return res;
    }
};
} // namespace ir