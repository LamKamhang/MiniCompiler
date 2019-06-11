#pragma once
#include "type.h"

namespace ir
{
class IntegerTy : public BaseType
{
public:
    int bits;
    bool is_sign;
    IntegerTy() = default;
    IntegerTy(int bits, bool is_sign, bool is_const = false) : bits(bits), BaseType(ir::IntegerTy::get(bits), ir::TypeName::Integer, is_const) {}

    llvm::Value *allocate(const std::string &name)
    {
        return builder->CreateAlloca(this->_ty, nullptr, name);
    }

    template <typename T>
    llvm::Value *castTo(const T *type, llvm::Value *value)
    {
        auto dest_type = dynamic_cast<ir::BaseType *>(type);
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
    static llvm::Type *get(int bits)
    {
        llvm::Type::getIntNTy(*context, bits);
    }
};
} // namespace ir