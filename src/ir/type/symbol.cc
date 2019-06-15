#include "symbol.h"
#include "iostream"
#include "memory"
#include "sstream"
std::shared_ptr<ir::Symbol> ir::Symbol::Error(const std::string &info)
{
    std::cout << "[symbol: " << this->name << "] " << info << std::endl;
    return nullptr;
}
ir::Symbol::Symbol(std::shared_ptr<ir::Type> type) : type(type), is_lvalue(false)
{
    std::stringstream ss;
    ss << "const_" << type->Top()->type_name << "_";
    this->name = ss.str();
}
ir::Symbol::Symbol(std::shared_ptr<ir::Type> type, const std::string &name, bool is_lvalue) : type(type), name(name)
{
    this->is_lvalue = is_lvalue;
    if (this->is_lvalue)
    {
        this->value = this->type->Allocate(this->name);
    }
}
std::shared_ptr<ir::Symbol> ir::Symbol::LValue()
{
    if (!this->is_lvalue)
        return this->Error("use a RValue as LValue.");
    auto res = std::make_shared<ir::Symbol>(this->type, this->name + "_LValue", true);
    res->is_lvalue = true;
    res->value = this->value;
    return res;
}
std::shared_ptr<ir::Symbol> ir::Symbol::RValue()
{
    auto res = std::make_shared<ir::Symbol>(this->type, this->name + "_RValue", false);
    if (this->is_lvalue)
    {
        res->value = builder->CreateLoad(this->value);
    }
    else
        res->value = this->value;
    return res;
}
std::shared_ptr<ir::Symbol> ir::Symbol::DeReference()
{
    if (this->is_lvalue)
        return (std::shared_ptr<ir::Symbol>)this->Error("deference a LValue.");
    auto res = std::make_shared<ir::Symbol>(this->type, this->name + "_RValue", false);
    return res->type->DeReference() ? res
                                    : this->Error("dereference a non-pointer type symbol.");
}
llvm::Value *ir::Symbol::GetValue()
{
    return this->value;
}
// val should be a RValue
llvm::Value *ir::Symbol::Store(llvm::Value *val)
{
    if (this->is_lvalue)
    {
        return builder->CreateStore(val, this->value);
    }
    else
    {
        return this->value = val;
    }
}
llvm::Value *ir::Symbol::Assign(std::shared_ptr<ir::Symbol> val)
{
    if (!this->is_lvalue)
        return this->Error("a RValue can't be assigned."), nullptr;
    auto rhs_symbol = val->RValue();
    auto rhs_type = val->type;
    auto rhs_val = rhs_symbol->value;
    // type check
    auto lhs_symbol = this;
    auto lhs_type = lhs_symbol->type;

    std::stringstream ss;
    if (lhs_type->_bty == rhs_type->_bty &&
        lhs_type->_tys.size() == rhs_type->_tys.size())
    {
        for (auto i = 0; i < lhs_type->_tys.size(); ++i)
        {
            auto l_ty = lhs_type->_tys[i];
            auto r_ty = rhs_type->_tys[i];
            if (l_ty->type_name != r_ty->type_name)
            {
                ss << "type \' " << lhs_type->TyInfo() << "\' and \' " << rhs_type->TyInfo() << "\' is not compatible to assign.\n";
                goto fail;
            }
            // if top level is const
            else if (i + 1 == lhs_type->_tys.size() && l_ty->is_const)
            {
                ss << "\'" << rhs_type->TyInfo() << "\' cannot assign to variable with const-qualified type \'" << lhs_type->TyInfo() << "\'\n";
                goto fail;
            }
        }
        return this->Store(rhs_val);
    }
fail:
    return this->Error(ss.str()), nullptr;
}
bool ir::Symbol::IsValid()
{
    return this->value != nullptr;
}
std::shared_ptr<ir::Symbol> ir::Symbol::Get(std::shared_ptr<ir::Type> type, const std::string &name)
{
    auto res = new ir::Symbol(type, name, true);
    return (std::shared_ptr<ir::Symbol>)res;
}
std::shared_ptr<ir::Symbol> ir::Symbol::GetConstant(std::shared_ptr<ir::Type> type, llvm::Value *val)
{
    auto res = std::make_shared<ir::Symbol>(type);
    res->value = val;
    return (std::shared_ptr<ir::Symbol>)res;
}
