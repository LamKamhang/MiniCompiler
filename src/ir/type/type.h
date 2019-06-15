#pragma once
#include "../ir.h"
#include <llvm/IR/Type.h>
#include <map>
#include <string>
namespace ir
{
enum TypeName
{
    Void,
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
    virtual std::string TyInfo() = 0;

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
    virtual std::string TyInfo() = 0;
    virtual llvm::Value *CastTo(llvm::Value *value) { return nullptr; };
}; // namespace ir

class ReferType : public RootType
{
protected:
    ReferType(TypeName type_name, bool is_const);

public:
    virtual std::string TyInfo() = 0;
};

class Type
{
public:
    Type() = default;
    BaseType *_bty;
    std::vector<ReferType *> _tys;
    llvm::Value *Allocate(const std::string &name);
    bool DeReference();
    ir::RootType *Top();
    ir::BaseType *BaseTy() { return this->_bty; }
    std::string TyInfo();
    std::shared_ptr<ir::Type> CastTo(std::shared_ptr<ir::Type> type);
    static llvm::Type *GetLlvmType(const std::string &type);
    static std::shared_ptr<ir::Type> Get(std::vector<ir::RootType *> &types);
    static std::shared_ptr<ir::Type> GetConstantType(const std::string &type);
};
} // namespace ir