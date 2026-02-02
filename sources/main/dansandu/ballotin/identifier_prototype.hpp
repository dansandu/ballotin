#pragma once

#include <ostream>
#include <string>

namespace dansandu::ballotin::identifier_prototype
{

template<typename Tag, typename Integer>
class IdentifierPrototype
{
public:
    friend constexpr auto operator<=>(const IdentifierPrototype& left, const IdentifierPrototype& right) = default;

    friend std::ostream& operator<<(std::ostream& stream, const IdentifierPrototype identifierPrototype)
    {
        return stream << identifierPrototype.integer_;
    }

    using IntegerType = Integer;

    constexpr IdentifierPrototype() : integer_{0}
    {
    }

    constexpr explicit IdentifierPrototype(const IntegerType integer) : integer_{integer}
    {
    }

    constexpr IntegerType getInteger() const
    {
        return integer_;
    }

    std::string toString() const
    {
        return std::to_string(integer_);
    }

private:
    IntegerType integer_;
};

}
