#pragma once
#include "../ir.h"
#include <llvm/IR/Type.h>
#include <map>
#include <string>
namespace ir
{
enum TypeName
{
    Integer,
    Float,
    Pointer,
    Array,
    Struct,
    Function,
    Enum
};
class RootType
{
public:
    TypeName type_name;
    bool is_const;
    virtual llvm::Value *Allocate(const std::string &name) = 0;
    virtual llvm::Value *CastTo(llvm::Value *value) = 0;

protected:
    RootType(TypeName type_name, bool is_const) : type_name(type_name), is_const(is_const){};
};

class BaseType : public RootType
{
protected:
    BaseType(llvm::Type *type, TypeName type_name, bool is_const);

public:
    llvm::Type *_ty;

    virtual llvm::Value *Allocate(const std::string &name) = 0;
    llvm::Value *CastTo(llvm::Value *value)
    {
        return nullptr;
    };
}; // namespace ir

class ReferType : public RootType
{
public:
    virtual llvm::Value *DeReference(llvm::Value *) = 0;
    virtual llvm::Value *IndexReference(int idx) = 0;
};

class Type
{
private:
    Type() = default;
    BaseType *_bty;
    std::vector<ReferType *> _tys;

public:
    llvm::Value *Allocate(const std::string &name);
    bool DeReference();
    ir::RootType *Top();
    ir::BaseType *BaseTy() { return this->_bty; }
    static ir::Type *Get(std::vector<ir::RootType *> &types);
    static llvm::Type *GetLlvmType(const std::string &type);
    static ir::Type *GetConstantType(const std::string &type);
};
} // namespace ir