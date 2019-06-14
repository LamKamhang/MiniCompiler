#include "symbol.h"
#include "memory"
ir::Symbol::Symbol(ir::Type *type, const std::string &name, bool is_lvalue) : type(type), name(name), is_lvalue(is_lvalue)
{
    if (this->is_lvalue)
    {
        this->value = this->type->allocate(this->name);
    }
}
std::shared_ptr<ir::Symbol> ir::Symbol::LValue()
{
    if (this->is_lvalue)
    {
        return (std::shared_ptr<ir::Symbol>)this;
    }
    else
    {
        return nullptr;
    }
}
std::shared_ptr<ir::Symbol> ir::Symbol::RValue()
{
    if (this->is_lvalue)
    {
        // auto res = std::make_shared<ir::Symbol>(this->type, this->name, false);
        // res->value = builder->CreateLoad(this->value);
        // return res;
        auto res = new ir::Symbol(this->type, this->name, false);
        res->value = builder->CreateLoad(this->value);
        return (std::shared_ptr<ir::Symbol>)res;
    }
    else
    {
        return (std::shared_ptr<ir::Symbol>)this;
    }
}
std::shared_ptr<ir::Symbol> ir::Symbol::get(ir::Type *type, const std::string &name)
{
    // auto res = std::make_shared<ir::Symbol>(type, name, true);
    // return res;
    auto res = new ir::Symbol(type, name, true);
    return (std::shared_ptr<ir::Symbol>)res;
}