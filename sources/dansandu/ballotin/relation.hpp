#pragma once

namespace dansandu::ballotin::relation
{

template<typename T>
struct total_order
{
    friend bool operator>(const T& left, const T& right)
    {
        return right < left;
    }

    friend bool operator<=(const T& left, const T& right)
    {
        return !(right < left);
    }

    friend bool operator>=(const T& left, const T& right)
    {
        return !(left < right);
    }

    friend bool operator==(const T& left, const T& right)
    {
        return !(left < right) & !(right < left);
    }

    friend bool operator!=(const T& left, const T& right)
    {
        return (left < right) | (right < left);
    }
};

}
