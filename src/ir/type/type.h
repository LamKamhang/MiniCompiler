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
    Enum
};
class RootType
{
public:
    TypeName type_name;
    virtual llvm::Value *allocate(const std::string &name) { return nullptr; }
    virtual llvm::Value *castTo(llvm::Value *value);

protected:
    RootType(TypeName type_name) : type_name(type_name){};
};

class BaseType : public RootType
{
protected:
    BaseType(llvm::Type *type, TypeName type_name, bool is_const) : _ty(type), RootType(type_name), is_const(is_const){};

public:
    llvm::Type *_ty;
    bool is_const;
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
    std::vector<ReferType *> _tys;
    Type(){};

public:
    llvm::Value *allocate(const std::string &name);
    static ir::Type *get(std::vector<ir::RootType *>);
    static llvm::Type *getConstantType(const std::string &type);
};
} // namespace ir