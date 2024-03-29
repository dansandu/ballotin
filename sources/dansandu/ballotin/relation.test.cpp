#include "dansandu/ballotin/relation.hpp"
#include "catchorg/catch/catch.hpp"

using dansandu::ballotin::relation::TotalOrder;

class Duck : public TotalOrder<Duck>
{
    friend bool operator<(const Duck& left, const Duck& right)
    {
        return left.weight_ < right.weight_;
    }

public:
    explicit Duck(int weight) : weight_{weight}
    {
    }

private:
    int weight_;
};

TEST_CASE("relation")
{
    SECTION("total order")
    {
        REQUIRE(Duck{1} < Duck{2});

        REQUIRE_FALSE(Duck{2} < Duck{1});

        REQUIRE(Duck{2} > Duck{1});

        REQUIRE_FALSE(Duck{1} > Duck{2});

        REQUIRE(Duck{1} <= Duck{2});

        REQUIRE_FALSE(Duck{2} <= Duck{1});

        REQUIRE(Duck{2} >= Duck{1});

        REQUIRE_FALSE(Duck{1} >= Duck{2});

        REQUIRE(Duck{1} != Duck{2});

        REQUIRE_FALSE(Duck{1} == Duck{2});

        REQUIRE(Duck{3} == Duck{3});

        REQUIRE_FALSE(Duck{3} != Duck{3});
    }
}
