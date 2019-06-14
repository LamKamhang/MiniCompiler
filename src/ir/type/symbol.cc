#include "symbol.h"
#include "iostream"
#include "memory"
#include "sstream"
std::shared_ptr<ir::Symbol> ir::Symbol::Error(const std::string &info)
{
    std::cout << "[symbol: " << this->name << "] " << info << std::endl;
    return nullptr;
}
ir::Symbol::Symbol(ir::Type *type) : type(type), is_lvalue(false)
{
    std::stringstream ss;
    ss << "const_" << type->Top()->type_name << "_";
    this->name = ss.str();
}
ir::Symbol::Symbol(ir::Type *type, const std::string &name, bool is_lvalue) : type(type), name(name)
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
    auto res = std::make_shared<ir::Symbol>(this->type, this->name + "_LValue", false);
    res->is_lvalue = true;
    res->value = this->value;
    return res;
}
std::shared_ptr<ir::Symbol> ir::Symbol::RValue()
{
    auto res = std::make_shared<ir::Symbol>(this->type, this->name + "_RValue", false);
    if (this->is_lvalue)
    {
        auto res = std::make_shared<ir::Symbol>(this->type, this->name + "_RValue", false);
        res->value = builder->CreateLoad(res->value);
    }
    else
        res->value = this->value;
    return res;
}
std::shared_ptr<ir::Symbol> ir::Symbol::DeReference()
{
    if (this->is_lvalue)
        return (std::shared_ptr<ir::Symbol>)this->Error("deference a LValue.");
    auto res = new ir::Symbol(this->type, this->name + "_RValue", false);
    return res->type->DeReference() ? (std::shared_ptr<ir::Symbol>)res
                                    : this->Error("dereference a non-pointer type symbol.");
}
// val should be a RValue
llvm::Value *ir::Symbol::Store(llvm::Value *val)
{
    if (this->is_lvalue)
    {
        // [not implement] type check
        // check if const
        return this->type->Top()->is_const ? this->Error("a const type can't be stored a new value."), nullptr
                                           : builder->CreateStore(val, this->value);
    }
    else
    {
        this->Error("a RValue can't be stored a value.");
        return nullptr;
    }
}
bool ir::Symbol::IsValid()
{
    return this->value != nullptr;
}
std::shared_ptr<ir::Symbol> ir::Symbol::Get(ir::Type *type, const std::string &name)
{
    auto res = new ir::Symbol(type, name, true);
    return (std::shared_ptr<ir::Symbol>)res;
}
std::shared_ptr<ir::Symbol> ir::Symbol::GetConstant(ir::Type *type, llvm::Value *val)
{
    auto res = new ir::Symbol(type);
    res->value = val;
    return (std::shared_ptr<ir::Symbol>)res;
}