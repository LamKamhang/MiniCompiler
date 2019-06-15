#pragma once
#include "type.h"

namespace ir
{
class Pointer : public ir::ReferType
{
private:
    Pointer() = default;

public:
    std::string TyInfo();
};
} // namespace ir