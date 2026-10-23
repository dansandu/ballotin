#pragma once

#include <chrono>
#include <future>
#include <vector>

namespace dansandu::ballotin::threading
{

template<typename T, typename R, typename P>
std::future_status waitAllFor(const std::vector<std::future<T>>& futures, const std::chrono::duration<R, P>& timeout)
{
    for (const auto& future : futures)
    {
        if (!future.valid())
        {
            throw std::future_error(std::future_errc::no_state);
        }
    }

    const auto deadline = std::chrono::steady_clock::now() + timeout;

    for (const auto& future : futures)
    {
        const auto status = future.wait_until(deadline);

        if (status != std::future_status::ready)
        {
            // Status is timeout or deferred.
            return status;
        }
    }

    return std::future_status::ready;
}

}
