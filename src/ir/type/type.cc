#include "type.h"
#include "integer.h"

// RootType

// BaseType
ir::BaseType::BaseType(llvm::Type *type, TypeName type_name, bool is_const) : _ty(type), RootType(type_name), is_const(is_const) {}

llvm::Type *ir::Type::getConstantType(const std::string &type)
{
    if (type == "int")
    {
        return llvm::Type::getInt32Ty(*context);
    }
    if (type == "char")
    {
        return llvm::Type::getInt8Ty(*context);
    }
    if (type == "float")
    {
        return llvm::Type::getFloatTy(*context);
    }
    if (type == "void")
    {
        return llvm::Type::getVoidTy(*context);
    }
    return nullptr;
}
llvm::Value *ir::Type::allocate(const std::string &name)
{
    return this->_bty->allocate(name);
}
ir::Type *ir::Type::get(std::vector<ir::RootType *> types)
{
    auto res = new ir::Type();
    res->_bty = static_cast<ir::BaseType *>(types[0]);
    return res;
}

// integer
ir::IntegerTy::IntegerTy(int bits, bool is_sign, bool is_const) : bits(bits), BaseType(ir::IntegerTy::get(bits), ir::TypeName::Integer, is_const) {}
llvm::Value *ir::IntegerTy::allocate(const std::string &name)
{
    return builder->CreateAlloca(this->_ty, nullptr, name);
}

template <typename T>
llvm::Value *ir::IntegerTy::castTo(const T *type, llvm::Value *value)
{
    auto dest_type = static_cast<ir::BaseType *>(type);
    if (!dest_type)
        return nullptr;
    auto type_name = dest_type->type_name;
    auto dest_ty = dest_type->_ty;
    switch (type_name)
    {
    case ir::TypeName::Float:
        return this->is_sign ? builder->CreateSIToFP(value, dest_ty, "si2f_tmp") : builder->CreateUIToFP(value, dest_ty, "ui2f_tmp");
        break;
    case ir::TypeName::Integer:
        auto dest_type = dynamic_cast<ir::IntegerTy *>(dest_type);
        return builder->CreateIntCast(value, dest_ty, dest_type->is_sign, "i2i_tmp");
        break;
    case ir::TypeName::Pointer:
        return builder->CreateIntToPtr(value, dest_ty, "i2p_tmp");
        break;
    default:
        return nullptr;
    }
}
llvm::Type *ir::IntegerTy::get(int bits)
{
    return llvm::Type::getIntNTy(*context, bits);
}