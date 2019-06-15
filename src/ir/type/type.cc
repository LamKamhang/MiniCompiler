#include "index.h"
#include <sstream>
#include <unordered_map>

// RootType

// BaseType
ir::BaseType::BaseType(llvm::Type *type, TypeName type_name, bool is_const) : _ty(type), RootType(type_name, is_const) {}

llvm::Type *ir::Type::GetLlvmType(const std::string &type)
{
    return type == "char"
               ? llvm::Type::getInt8Ty(*context)
               : type == "short"
                     ? llvm::Type::getInt16Ty(*context)
                     : type == "int"
                           ? llvm::Type::getInt32Ty(*context)
                           : type == "long"
                                 ? llvm::Type::getInt64Ty(*context)
                                 : type == "float"
                                       ? llvm::Type::getFloatTy(*context)
                                       : type == "double"
                                             ? llvm::Type::getDoubleTy(*context)
                                             : type == "void"
                                                   ? llvm::Type::getVoidTy(*context)
                                                   : nullptr;
}
std::shared_ptr<ir::Type> ir::Type::GetConstantType(const std::string &type)
{
    auto res = std::make_shared<ir::Type>();
    // [not implement] all other types
    res->_bty = type == "int" ? ir::IntegerTy::Get(32, true, true) : nullptr;
    return res;
}
llvm::Value *ir::Type::Allocate(const std::string &name)
{
    return this->Top()->Allocate(name);
}
std::shared_ptr<ir::Type> ir::Type::Get(std::vector<ir::RootType *> &types)
{
    auto res = std::make_shared<ir::Type>();
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
std::string ir::Type::TyInfo()
{
    std::stringstream ss;
    auto base_type = this->_bty;
    auto types = this->_tys;
    ss << base_type->TyInfo() << " ";
    for (auto ty : types)
    {
        ss << ty->TyInfo() << " ";
    }
    return ss.str();
}
std::shared_ptr<ir::Type> ir::Type::CastTo(std::shared_ptr<ir::Type> type)
{
    // type check
    auto from_type = this;
    auto from_base = from_type->_bty;
    auto &from_tys = from_type->_tys;

    auto to_type = type;
    auto to_base = to_type->_bty;
    auto &to_tys = to_type->_tys;

    bool valid = true;
    std::stringstream ss;
    ss << "type \' " << from_type->TyInfo() << "\' ==> \' " << to_type->TyInfo() << "\' is not compatible.\n";

    // a small type can be cast to a bigger type
    // void* can cast to any ptr, and there can't be sth like void****
    if (from_base->type_name == ir::TypeName::Void)
    {
        valid = from_type->Top()->type_name == ir::Pointer &&
                to_type->Top()->type_name == ir::Pointer &&
                from_tys.size() <= to_tys.size();
    }
    // array type cann't be cast
    else if (to_type->Top()->type_name == ir::TypeName::Array || from_type->Top()->type_name == ir::TypeName::Array)
    {
        valid = false;
    }
    // int->pointer
    else if (dynamic_cast<ir::IntegerTy *>(from_base) && to_type->Top()->type_name == ir::TypeName::Pointer)
    {
        valid = true;
    }
    // in other case
    else if (from_type->_tys.size() == to_type->_tys.size())
    {
        // if it's a base type cast
        if (from_type->_tys.size() == 0)
        {
            valid = true;
        }
        // else it has to be a non-const to const cast
        else
        {
            for (auto i = 0; i < from_tys.size(); ++i)
            {
                // the n-1 level must be same
                auto f_ty = from_tys[i];
                auto t_ty = to_tys[i];
                // if not top level, const qualifier must be same
                if (f_ty->type_name != t_ty->type_name ||
                    i + 1 != from_tys.size() && f_ty->is_const != t_ty->is_const)
                {
                    valid = false;
                    break;
                }
                // the top level: from: non-const, to: const
                else if (f_ty->is_const || !t_ty->is_const)
                {
                    ss << "non-const type cannot cast to const type.\n";
                    valid = false;
                }
            }
        }
    }
    else
    {
        ss << "type length not match.\n";
    }

    if (valid)
    {
        auto res = std::make_shared<ir::Type>();
        res->_bty = to_base;
        res->_tys = to_tys;
        return res;
    }
    else
    {

        return llvm::errs() << ss.str(), nullptr;
    }
}

// [IntegerTy]
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
std::string ir::IntegerTy::TyInfo()
{
    auto bits = this->bits;
    auto is_sign = this->is_sign;
    std::stringstream ss;
    ss << is_sign ? "unsigned" : "";
    ss << " " << (bits == 8)
        ? "char"
        : (bits == 16)
              ? "short"
              : (bits == 32)
                    ? "int"
                    : (bits == 64)
                          ? "long"
                          : (bits == 128)
                                ? "long long"
                                : "";
    return ss.str();
}

// [ReferType]
ir::ReferType::ReferType(TypeName type_name, bool is_const) : ir::RootType(type_name, is_const) {}

// [Function]
ir::FunctionTy::FunctionTy(bool defined, const std::string &name) : defined(defined), ir::RootType(ir::TypeName::Function, true), name(name) {}
std::shared_ptr<ir::FunctionTy> ir::FunctionTy::Get(bool defined, const std::string &name, std::vector<std::shared_ptr<ir::Type>> &types)
{
    auto res = std::make_shared<ir::FunctionTy>(defined, name);
    res->ret_type = types[0];
    res->para_type.insert(res->para_type.end(), types.begin() + 1, types.end());
    return res;
}
llvm::Value *ir::FunctionTy::Allocate(const std::string &name)
{
    return nullptr;
}
std::string ir::FunctionTy::TyInfo()
{
    std::stringstream ss;
    ss << this->ret_type->TyInfo();
    ss << " " << this->name << " ( ";
    for (auto para : this->para_type)
    {
        ss << para->TyInfo() << ", ";
    }
    ss << ")";
    return ss.str();
}

// [Pointer]
std::string ir::Pointer::TyInfo()
{
    return "*";
}