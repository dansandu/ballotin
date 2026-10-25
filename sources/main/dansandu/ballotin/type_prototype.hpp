#pragma once

#include "dansandu/journey/exception.hpp"

#include <concepts>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>

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
    safeSizeTypeConversion = 256,
    all = 0xFFFFU
};

template<typename Tag, typename Underlying,
         std::underlying_type_t<TypeFeature> features =
             (TypeFeature::underlyingConversion | TypeFeature::stringConversion | TypeFeature::equality),
         Underlying defaultValue = 0>
    requires(std::is_arithmetic_v<Underlying>)
class TypePrototype
{
    constexpr static bool hasFeatures(const unsigned flags) noexcept
    {
        return static_cast<bool>(features & flags);
    }

public:
    using UnderlyingType = Underlying;

    friend constexpr bool operator==(const TypePrototype& left, const TypePrototype& right) noexcept
        requires(hasFeatures(TypeFeature::equality))
    {
        return left.underlying_ == right.underlying_;
    }

    friend constexpr bool operator!=(const TypePrototype& left, const TypePrototype& right) noexcept
        requires(hasFeatures(TypeFeature::equality))
    {
        return left.underlying_ != right.underlying_;
    }

    friend constexpr bool operator<(const TypePrototype& left, const TypePrototype& right) noexcept
        requires(hasFeatures(TypeFeature::inequality))
    {
        return left.underlying_ < right.underlying_;
    }

    friend constexpr bool operator>(const TypePrototype& left, const TypePrototype& right) noexcept
        requires(hasFeatures(TypeFeature::inequality))
    {
        return left.underlying_ > right.underlying_;
    }

    friend constexpr bool operator<=(const TypePrototype& left, const TypePrototype& right) noexcept
        requires(hasFeatures(TypeFeature::inequality))
    {
        return left.underlying_ <= right.underlying_;
    }

    friend constexpr bool operator>=(const TypePrototype& left, const TypePrototype& right) noexcept
        requires(hasFeatures(TypeFeature::inequality))
    {
        return left.underlying_ >= right.underlying_;
    }

    friend constexpr TypePrototype operator+(const TypePrototype& left, const TypePrototype& right) noexcept
        requires(hasFeatures(TypeFeature::addition))
    {
        return TypePrototype{left.underlying_ + right.underlying_};
    }

    friend constexpr TypePrototype operator-(const TypePrototype& left, const TypePrototype& right) noexcept
        requires(hasFeatures(TypeFeature::subtraction))
    {
        return TypePrototype{left.underlying_ - right.underlying_};
    }

    friend constexpr TypePrototype operator*(const TypePrototype& left, const TypePrototype& right) noexcept
        requires(hasFeatures(TypeFeature::multiplication))
    {
        return TypePrototype{left.underlying_ * right.underlying_};
    }

    friend constexpr TypePrototype operator/(const TypePrototype& left, const TypePrototype& right) noexcept
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

    static constexpr TypePrototype fromSizeType(const size_t size)
        requires(hasFeatures(TypeFeature::safeSizeTypeConversion) && std::integral<UnderlyingType>)
    {
        if (!std::in_range<UnderlyingType>(size))
        {
            THROW(std::out_of_range, "The size ", size, " exceeds the maximum value of the underlying type");
        }

        return TypePrototype{static_cast<UnderlyingType>(size)};
    }

    constexpr TypePrototype() noexcept : underlying_{defaultValue}
    {
    }

    constexpr explicit TypePrototype(const UnderlyingType underlying) noexcept : underlying_{underlying}
    {
    }

    constexpr TypePrototype& operator++() noexcept
        requires(hasFeatures(TypeFeature::addition))
    {
        ++underlying_;
        return *this;
    }

    constexpr TypePrototype operator++(int) noexcept
        requires(hasFeatures(TypeFeature::addition))
    {
        const auto copy = *this;
        ++underlying_;
        return copy;
    }

    constexpr TypePrototype& operator+=(const TypePrototype& other) noexcept
        requires(hasFeatures(TypeFeature::addition))
    {
        underlying_ += other.underlying_;
        return *this;
    }

    constexpr TypePrototype& operator--() noexcept
        requires(hasFeatures(TypeFeature::subtraction))
    {
        --underlying_;
        return *this;
    }

    constexpr TypePrototype operator--(int) noexcept
        requires(hasFeatures(TypeFeature::subtraction))
    {
        const auto copy = *this;
        --underlying_;
        return copy;
    }

    constexpr TypePrototype& operator-=(const TypePrototype& other) noexcept
        requires(hasFeatures(TypeFeature::subtraction))
    {
        underlying_ -= other.underlying_;
        return *this;
    }

    constexpr TypePrototype& operator*=(const TypePrototype& other) noexcept
        requires(hasFeatures(TypeFeature::multiplication))
    {
        underlying_ *= other.underlying_;
        return *this;
    }

    constexpr TypePrototype& operator/=(const TypePrototype& other) noexcept
        requires(hasFeatures(TypeFeature::division))
    {
        underlying_ /= other.underlying_;
        return *this;
    }

    constexpr UnderlyingType getUnderlying() const noexcept
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
