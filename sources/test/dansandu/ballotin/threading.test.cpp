#include "dansandu/ballotin/threading.hpp"
#include "dansandu/radiance/radiance.hpp"

#include <chrono>
#include <future>
#include <vector>

using dansandu::ballotin::threading::waitAllFor;

TEST_CASE("threading")
{
    SECTION("waitAllFor")
    {
        SECTION("returns ready if promises set")
        {
            auto firstPromise = std::promise<int>{};

            auto secondPromise = std::promise<int>{};

            auto futures = std::vector<std::future<int>>{};

            futures.push_back(firstPromise.get_future());

            futures.push_back(secondPromise.get_future());

            firstPromise.set_value(100);

            secondPromise.set_value(200);

            const auto timeout = std::chrono::milliseconds(200);

            const auto isReady = (waitAllFor(futures, timeout) == std::future_status::ready);

            REQUIRE(isReady);

            REQUIRE(futures.at(0).get() == 100);

            REQUIRE(futures.at(1).get() == 200);
        }

        SECTION("returns timeout if promises not set")
        {
            auto firstPromise = std::promise<int>{};

            auto secondPromise = std::promise<int>{};

            auto futures = std::vector<std::future<int>>{};

            futures.push_back(firstPromise.get_future());

            futures.push_back(secondPromise.get_future());

            const auto timeout = std::chrono::milliseconds(200);

            const auto isTimeout = (waitAllFor(futures, timeout) == std::future_status::timeout);

            REQUIRE(isTimeout);
        }

        SECTION("throws if invalid future")
        {
            auto futures = std::vector<std::future<int>>{};

            futures.push_back({});

            futures.push_back({});

            const auto timeout = std::chrono::milliseconds(200);

            REQUIRE_THROW(std::future_error, waitAllFor(futures, timeout));
        }
    }
}
