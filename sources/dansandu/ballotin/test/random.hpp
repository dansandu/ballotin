#pragma once

namespace dansandu::ballotin::test::random
{

class PredictableBitGenerator
{
public:
    using result_type = unsigned;

    PredictableBitGenerator() : index_{0U}
    {
    }

    result_type operator()()
    {
        auto result = index_;
        index_ = (index_ + 1U) % (max() + 1U);
        return result;
    }

    constexpr static result_type min()
    {
        return 0U;
    }

    constexpr static result_type max()
    {
        return 9U;
    }

private:
    result_type index_;
};

}
