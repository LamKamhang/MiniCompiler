#include "../global.h"
#include "index.h"
#include <llvm/Support/raw_ostream.h>
#include <sstream>
#include <unordered_map>

// BaseType
ir::BaseType::BaseType(llvm::Type *type, TypeName type_name, bool is_const) : RootType(type_name, type, is_const) {}

llvm::Type *ir::Type::GetLlvmType(const std::string &type)
{
    return type == "bool"
               ? llvm::Type::getInt1Ty(*context)
               : type == "char"
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
    res->_bty =
        type == "void"
            ? dynamic_cast<ir::BaseType *>(ir::VoidTy::Get())
            : type == "bool"
                  ? dynamic_cast<ir::BaseType *>(ir::IntegerTy::Get(1, false, true))
                  : type == "char"
                        ? dynamic_cast<ir::BaseType *>(ir::IntegerTy::Get(8, false, true))
                        : type == "int"
                              ? dynamic_cast<ir::BaseType *>(ir::IntegerTy::Get(32, true, true))
                              : type == "float"
                                    ? dynamic_cast<ir::BaseType *>(ir::FloatTy::Get(32, true))
                                    : nullptr;
    return res;
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

    auto res = std::make_shared<ir::Type>();
    res->_bty = to_base;
    res->_tys = to_tys;

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
            if (from_base->type_name == ir::TypeName::Float || to_base->type_name == ir::TypeName::Float)
            {
                res->_bty = from_base->type_name == ir::TypeName::Float ? from_base : to_base;
            }
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

        return res;
    }
    else
    {
        return llvm::errs() << ss.str(), nullptr;
    }
}
bool ir::Type::Equal(std::shared_ptr<ir::Type> type)
{
    auto a = this;
    auto b = type;
    if (a->_bty->type_name != b->_bty->type_name || a->_bty->_ty != b->_bty->_ty || a->_tys.size() != b->_tys.size())
        return false;
    for (auto i = 0; i < a->_tys.size(); ++i)
    {
        auto a_ty = a->_tys[i];
        auto b_ty = b->_tys[i];
        if (a_ty->_ty != b_ty->_ty || a_ty->is_const != b_ty->is_const)
            return false;
    }
    return true;
}

// [IntegerTy]
static std::unordered_map<int, std::shared_ptr<ir::IntegerTy>> IntManager;
llvm::Type *ir::IntegerTy::GetBitType(int bits)
{
    return llvm::IntegerType::getIntNTy(*context, bits);
}
ir::IntegerTy::IntegerTy(int bits, bool is_sign, bool is_const) : bits(bits), BaseType(ir::IntegerTy::GetBitType(bits), ir::TypeName::Integer, is_const) {}

llvm::Value *ir::IntegerTy::CastTo(ir::RootType *type, llvm::Value *value)
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
    int is_sign_bit = !is_sign ? bits << 2 : bits << 1;
    int is_const_bit = is_const ? bits << 4 : bits << 3;
    int real_bit = bits | is_sign_bit | is_const_bit;
    int type_id = bits | is_sign_bit;
    // int real_bit = bits | is_sign_bit | is_const_bit;
    if (!IntManager.count(real_bit))
    {
        auto res = std::make_shared<ir::IntegerTy>(bits, is_sign, is_const);
        res->type_id = type_id;
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
    ss << " " << (bits == 1)
        ? "bool"
        : (bits == 8)
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

// [FloatTy]
static std::unordered_map<int, std::shared_ptr<ir::FloatTy>> FloatManager;
llvm::Type *ir::FloatTy::GetBitType(int bits)
{
    return bits == 32
               ? llvm::Type::getFloatTy(*context)
               : bits == 64
                     ? llvm::Type::getDoubleTy(*context)
                     : nullptr;
}
ir::FloatTy::FloatTy(int bits, bool is_const) : bits(bits), ir::BaseType(ir::FloatTy::GetBitType(bits), ir::TypeName::Float, is_const) {}

llvm::Value *ir::FloatTy::CastTo(ir::RootType *type, llvm::Value *value)
{
    auto dest_type = static_cast<ir::BaseType *>(type);
    llvm::Value *res = nullptr;
    if (!dest_type)
        return nullptr;
    auto type_name = dest_type->type_name;
    auto dest_ty = dest_type->_ty;
    auto i_ty = dynamic_cast<ir::IntegerTy *>(dest_type);

    switch (type_name)
    {
    case ir::TypeName::Float:
        res = builder->CreateFPCast(value, dest_ty, "f2f_tmp");
        break;
    case ir::TypeName::Integer:
        res = i_ty->is_sign ? builder->CreateFPToSI(value, i_ty->_ty, "f2si_tmp") : builder->CreateFPToUI(value, i_ty->_ty, "f2ui_tmp");
        break;
    default:
        break;
    }
    return res;
}
ir::FloatTy *ir::FloatTy::Get(int bits, bool is_const)
{
    int is_sign = 1;
    int is_sign_bit = !is_sign ? bits << 2 : bits << 1;
    int is_const_bit = is_const ? bits << 4 : bits << 3;
    int real_bit = bits | is_sign_bit | is_const_bit;
    int type_id = bits | is_sign_bit;
    if (!FloatManager.count(real_bit))
    {
        auto res = std::make_shared<ir::FloatTy>(bits, is_const);
        res->type_id = type_id;
        FloatManager[real_bit] = std::move(res);
    }
    return FloatManager.at(real_bit).get();
}
std::string ir::FloatTy::TyInfo()
{
    auto bits = this->bits;
    std::stringstream ss;
    ss << " " << (bits == 16)
        ? "float"
        : (bits == 32)
              ? "double"
              : "";
    return ss.str();
}
std::string ir::VoidTy::TyInfo()
{
    return "void";
}
static std::shared_ptr<ir::VoidTy> vty = nullptr;
ir::VoidTy *ir::VoidTy::Get()
{
    vty = std::make_shared<ir::VoidTy>();
    return vty.get();
}

// [ReferType]
ir::ReferType::ReferType(llvm::Type *type, TypeName type_name, bool is_const) : ir::RootType(type_name, type, is_const) {}

// [Function]
ir::FunctionTy::FunctionTy(bool defined, const std::string &name) : defined(defined), ir::RootType(ir::TypeName::Function, nullptr, true), name(name) {}
std::shared_ptr<ir::FunctionTy> ir::FunctionTy::Get(bool defined, const std::string &name, std::vector<std::shared_ptr<ir::Type>> &types)
{
    auto res = std::make_shared<ir::FunctionTy>(defined, name);
    res->ret_type = types[0];
    res->para_type.insert(res->para_type.end(), types.begin() + 1, types.end());
    return res;
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
bool ir::FunctionTy::Equal(std::shared_ptr<ir::FunctionTy> function)
{
    auto that_fun = this;
    if (!that_fun->ret_type->Equal(function->ret_type))
        return false;
    auto &a_para = function->para_type;
    auto &b_para = that_fun->para_type;
    if (a_para.size() != b_para.size())
        return false;
    for (auto i = 0; i < a_para.size(); ++i)
    {
        auto a_ty = a_para[i];
        auto b_ty = b_para[i];
        if (!a_ty->Equal(b_ty))
            return false;
    }
    return true;
}

// [Pointer]
std::string ir::Pointer::TyInfo()
{
    return "*";
}