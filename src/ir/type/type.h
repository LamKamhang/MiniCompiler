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
    llvm::Type *_ty;
    bool is_const;
    virtual std::string TyInfo() = 0;

protected:
    RootType(TypeName type_name, llvm::Type *type, bool is_const) : _ty(type), type_name(type_name), is_const(is_const){};
};

class BaseType : public RootType
{
protected:
    BaseType(llvm::Type *type, TypeName type_name, bool is_const);

public:
    int type_id;
    virtual std::string TyInfo() = 0;

}; // namespace ir

class ReferType : public RootType
{
protected:
    ReferType(llvm::Type *type, TypeName type_name, bool is_const);

public:
    virtual std::string TyInfo() = 0;
};

class Type
{
public:
    Type() = default;
    BaseType *_bty;
    std::vector<ReferType *> _tys;
    bool DeReference();
    ir::RootType *Top();
    ir::BaseType *BaseTy() { return this->_bty; }
    std::string TyInfo();
    bool Equal(std::shared_ptr<ir::Type> type);
    std::shared_ptr<ir::Type> CastTo(std::shared_ptr<ir::Type> type);
    static llvm::Type *GetLlvmType(const std::string &type);
    static std::shared_ptr<ir::Type> Get(std::vector<ir::RootType *> &types);
    static std::shared_ptr<ir::Type> GetConstantType(const std::string &type);
};
} // namespace ir