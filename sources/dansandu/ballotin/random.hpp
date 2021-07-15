#pragma once

namespace dansandu::ballotin::random
{

class PredictableBitGenerator
{
public:
    using result_type = unsigned;

    PredictableBitGenerator(result_type seed = 0U) : index_{seed % (max() + 1U)}
    {
    }

    void seed(result_type seed)
    {
        index_ = seed % (max() + 1U);
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
        return 255U;
    }

private:
    result_type index_;
};

}
