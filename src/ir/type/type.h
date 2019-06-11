#pragma once
#include "../ir.h"
#include <llvm/IR/Type.h>
#include <map>
#include <stack>
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
    Enum
};
class RootType
{
public:
    TypeName type_name;

protected:
    RootType(TypeName type_name) : type_name(type_name){};
};

class BaseType : public RootType
{
public:
    llvm::Type *_ty;
    bool is_const;
    BaseType(llvm::Type *type, TypeName type_name, bool is_const) : _ty(type), RootType(type_name), is_const(is_const){};
    virtual llvm::Value *allocate(const std::string &name) { return nullptr; }
    virtual llvm::Value *castTo(llvm::Value *value);
};

class ReferType : public RootType
{
public:
    virtual llvm::Value *refer(llvm::Value *);
    virtual llvm::Value *index(int idx);
};

class Type
{
private:
    BaseType *_bty;
    std::stack<ReferType *> _tys;

public:
    Type(){};
    llvm::Value *allocate(const std::string &name);
    static llvm::Type *getConstantType(const std::string &type);
};
} // namespace ir