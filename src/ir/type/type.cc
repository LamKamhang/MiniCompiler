#include "type.h"
#include "integer.h"
#include "unordered_map"

// RootType

// BaseType
ir::BaseType::BaseType(llvm::Type *type, TypeName type_name, bool is_const) : _ty(type), RootType(type_name, is_const) {}

llvm::Type *ir::Type::GetLlvmType(const std::string &type)
{
    return type == "int"
               ? llvm::Type::getInt32Ty(*context)
               : type == "char"
                     ? llvm::Type::getInt8Ty(*context)
                     : type == "float"
                           ? llvm::Type::getFloatTy(*context)
                           : type == "double"
                                 ? llvm::Type::getDoubleTy(*context)
                                 : type == "void"
                                       ? llvm::Type::getVoidTy(*context)
                                       : nullptr;
}
ir::Type *ir::Type::GetConstantType(const std::string &type)
{
    auto res = new ir::Type();
    // [not implement] all other types
    res->_bty = type == "int" ? ir::IntegerTy::Get(32, true, true) : nullptr;
    return res;
}
llvm::Value *ir::Type::Allocate(const std::string &name)
{
    return this->_bty->Allocate(name);
}
ir::Type *ir::Type::Get(std::vector<ir::RootType *> &types)
{
    auto res = new ir::Type();
    res->_bty = dynamic_cast<ir::BaseType *>(types[0]);
    for (auto i = types.begin() + 1; i != types.end(); ++i)
        res->_tys.push_back((ir::ReferType *)*i);
    return res;
}
bool ir::Type::DeReference()
{
    if (this->_tys.size())
    {
        this->_tys.pop_back();
        return true;
    }
    return false;
}
ir::RootType *ir::Type::Top()
{
    if (this->_tys.size())
        return (ir::RootType *)this->_tys.back();
    else
        return this->_bty;
}
// integer
static std::unordered_map<int, std::shared_ptr<ir::IntegerTy>> IntManager;
llvm::Type *ir::IntegerTy::GetBitType(int bits)
{
    // return builder->getInt32Ty();
    return llvm::Type::getInt32Ty(*context);
    // return builder->getIntNTy(bits);
}
ir::IntegerTy::IntegerTy(int bits, bool is_sign, bool is_const) : bits(bits), BaseType(ir::IntegerTy::GetBitType(bits), ir::TypeName::Integer, is_const) {}
llvm::Value *ir::IntegerTy::Allocate(const std::string &name)
{
    return builder->CreateAlloca(this->_ty, nullptr, name);
}

template <typename T>
llvm::Value *ir::IntegerTy::CastTo(const T *type, llvm::Value *value)
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
        return builder->CreateIntCast(value, dest_type->_ty, ((ir::IntegerTy *)dest_type)->is_sign, "i2i_tmp");
        break;
    case ir::TypeName::Pointer:
        return builder->CreateIntToPtr(value, dest_ty, "i2p_tmp");
        break;
    default:
        return nullptr;
        break;
    }
    return nullptr;
}
ir::IntegerTy *ir::IntegerTy::Get(int bits, bool is_sign, bool is_const)
{
    // bits is assumed to be less than 128, 2^8, 9 bit
    int is_sign_bit = (is_sign ? 1 : 0) << 10;
    int is_const_bit = (is_const ? 1 : 0) << 11;
    int real_bit = bits | is_sign_bit | is_const_bit;
    if (!IntManager.count(real_bit))
    {
        auto res = std::make_shared<ir::IntegerTy>(bits, is_sign, is_const);
        IntManager[real_bit] = std::move(res);
    }
    return IntManager.at(real_bit).get();
}