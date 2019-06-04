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
    Block *parent = nullptr;
    llvm::BasicBlock *basic_block;
    Block() = default;
    Block(Block *parent) : parent(parent){};
    // template <typename... T>
    // static const ir::Type &getCustomType(const ir::Block &block, T... args)
    // {
    //     const ir::Type type;
    //     const ir::Type &res = type;
    //     type.typeStack.push(ir::Type::getInt32Ty(ir::Context));
    //     block.TypeTable.insert({"int", std::move(type)});
    //     auto &context = ir::Context;
    //     auto &typeMap = ir::TypeMap;
    //     return res;
    // }
    llvm::Value *getSymbol(const std::string &name)
    {
        Block *node = this;
        while (node)
        {
            auto val = node->SymbolTable.at(name);
            if (val)
            {
                return val;
            }
            node = node->parent;
        }
        return nullptr;
    }
    static llvm::Type *getCustomType(const ir::Block &block, const std::string &type)
    {
        if (type == "int")
        {
            return llvm::Type::getInt32Ty(*ir::Context);
        }
        if (type == "char")
        {
            return llvm::Type::getInt8Ty(*ir::Context);
        }
    }
};
} // namespace ir