#include "type.h"

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
    res->_bty = dynamic_cast<ir::BaseType *>(types[0]);
    // for(auto i = types.begin()+1; i!=types.end();++i)
    // {
    //     res->_tys
    // }
    return res;
}