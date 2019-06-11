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
}
std::shared_ptr<ir::Symbol> ir::Symbol::RValue()
{
    if (this->is_lvalue)
    {
        // auto res = std::make_shared<ir::Symbol>(this->type, this->name, false);
        // res->value = this->RValue();
        // return res;
        return this->RValue();
    }
    else
    {
        return (std::shared_ptr<ir::Symbol>)this;
        // return std::make_shared<ir::Symbol>(this);
    }
}
std::shared_ptr<ir::Symbol> ir::Symbol::get(std::vector<std::pair<bool, ir::TypeName>> types)
{
    return nullptr;
    // auto base_type = types[0];
    // auto type = dynamic_cast<ir::BaseType>types[0];
    // return std::make_shared<ir::Symbol>();
}