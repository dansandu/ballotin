#pragma once

#include <concepts>
#include <ostream>
#include <string>

namespace dansandu::ballotin::type_prototype
{

enum TypeFeature : unsigned
{
    underlyingConversion = 1,
    stringConversion = 2,
    equality = 4,
    inequality = 8,
    addition = 16,
    subtraction = 32,
    multiplication = 64,
    division = 128,
    all = 0xFFFFU
};

template<typename Tag, typename Underlying,
         unsigned features =
             (TypeFeature::underlyingConversion | TypeFeature::stringConversion | TypeFeature::equality),
         Underlying defaultValue = 0>
class TypePrototype
{
    constexpr static bool hasFeatures(const unsigned flags)
    {
        return static_cast<bool>(features & flags);
    }

public:
    friend constexpr bool operator==(const TypePrototype& left, const TypePrototype& right)
        requires(hasFeatures(TypeFeature::equality))
    {
        return left.underlying_ == right.underlying_;
    }

    friend constexpr bool operator!=(const TypePrototype& left, const TypePrototype& right)
        requires(hasFeatures(TypeFeature::equality))
    {
        return left.underlying_ != right.underlying_;
    }

    friend constexpr bool operator<(const TypePrototype& left, const TypePrototype& right)
        requires(hasFeatures(TypeFeature::inequality))
    {
        return left.underlying_ < right.underlying_;
    }

    friend constexpr bool operator>(const TypePrototype& left, const TypePrototype& right)
        requires(hasFeatures(TypeFeature::inequality))
    {
        return left.underlying_ > right.underlying_;
    }

    friend constexpr bool operator<=(const TypePrototype& left, const TypePrototype& right)
        requires(hasFeatures(TypeFeature::inequality))
    {
        return left.underlying_ <= right.underlying_;
    }

    friend constexpr bool operator>=(const TypePrototype& left, const TypePrototype& right)
        requires(hasFeatures(TypeFeature::inequality))
    {
        return left.underlying_ >= right.underlying_;
    }

    friend constexpr TypePrototype operator+(const TypePrototype& left, const TypePrototype& right)
        requires(hasFeatures(TypeFeature::addition))
    {
        return TypePrototype{left.underlying_ + right.underlying_};
    }

    friend constexpr TypePrototype operator-(const TypePrototype& left, const TypePrototype& right)
        requires(hasFeatures(TypeFeature::subtraction))
    {
        return TypePrototype{left.underlying_ - right.underlying_};
    }

    friend constexpr TypePrototype operator*(const TypePrototype& left, const TypePrototype& right)
        requires(hasFeatures(TypeFeature::multiplication))
    {
        return TypePrototype{left.underlying_ * right.underlying_};
    }

    friend constexpr TypePrototype operator/(const TypePrototype& left, const TypePrototype& right)
        requires(hasFeatures(TypeFeature::division))
    {
        return TypePrototype{left.underlying_ / right.underlying_};
    }

    friend inline std::ostream& operator<<(std::ostream& stream, const TypePrototype& integerPrototype)
        requires(hasFeatures(TypeFeature::stringConversion))
    {
        return stream << integerPrototype.underlying_;
    }

    friend inline std::wostream& operator<<(std::wostream& stream, const TypePrototype& integerPrototype)
        requires(hasFeatures(TypeFeature::stringConversion))
    {
        return stream << integerPrototype.underlying_;
    }

    using UnderlyingType = Underlying;

    constexpr TypePrototype() : underlying_{defaultValue}
    {
    }

    constexpr explicit TypePrototype(const UnderlyingType underlying) : underlying_{underlying}
    {
    }

    constexpr TypePrototype& operator++()
        requires(hasFeatures(TypeFeature::addition))
    {
        ++underlying_;
        return *this;
    }

    constexpr TypePrototype operator++(int)
        requires(hasFeatures(TypeFeature::addition))
    {
        const auto copy = *this;
        ++underlying_;
        return copy;
    }

    constexpr TypePrototype& operator+=(const TypePrototype& other)
        requires(hasFeatures(TypeFeature::addition))
    {
        underlying_ += other.underlying_;
        return *this;
    }

    constexpr TypePrototype& operator--()
        requires(hasFeatures(TypeFeature::subtraction))
    {
        --underlying_;
        return *this;
    }

    constexpr TypePrototype operator--(int)
        requires(hasFeatures(TypeFeature::subtraction))
    {
        const auto copy = *this;
        --underlying_;
        return copy;
    }

    constexpr TypePrototype& operator-=(const TypePrototype& other)
        requires(hasFeatures(TypeFeature::subtraction))
    {
        underlying_ -= other.underlying_;
        return *this;
    }

    constexpr TypePrototype& operator*=(const TypePrototype& other)
        requires(hasFeatures(TypeFeature::multiplication))
    {
        underlying_ *= other.underlying_;
        return *this;
    }

    constexpr TypePrototype& operator/=(const TypePrototype& other)
        requires(hasFeatures(TypeFeature::division))
    {
        underlying_ /= other.underlying_;
        return *this;
    }

    constexpr UnderlyingType getUnderlying() const
        requires(hasFeatures(TypeFeature::underlyingConversion))
    {
        return underlying_;
    }

    std::string toString() const
        requires(hasFeatures(TypeFeature::stringConversion))
    {
        return std::to_string(underlying_);
    }

    std::wstring toWideString() const
        requires(hasFeatures(TypeFeature::stringConversion))
    {
        return std::to_wstring(underlying_);
    }

    void toStream(std::ostream& stream)
        requires(hasFeatures(TypeFeature::stringConversion))
    {
        stream << underlying_;
    }

    void toStream(std::wostream& stream)
        requires(hasFeatures(TypeFeature::stringConversion))
    {
        stream << underlying_;
    }

private:
    UnderlyingType underlying_;
};

}
