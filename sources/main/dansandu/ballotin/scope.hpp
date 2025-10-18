#pragma once

#include "dansandu/journey/macro.hpp"

#include <exception>
#include <type_traits>

namespace dansandu::ballotin::scope
{

template<typename FunctorType>
class ScopeFailureGuard
{
public:
    template<typename F>
    explicit ScopeFailureGuard(F&& functor)
        : uncaughtExceptions_{std::uncaught_exceptions()}, functor_{std::forward<F>(functor)}
    {
    }

    ScopeFailureGuard(const ScopeFailureGuard&) = delete;
    ScopeFailureGuard(ScopeFailureGuard&&) = delete;
    ScopeFailureGuard& operator=(const ScopeFailureGuard&) = delete;
    ScopeFailureGuard& operator=(ScopeFailureGuard&&) = delete;

    ~ScopeFailureGuard() noexcept
    {
        const auto currentUncaughtExceptions = std::uncaught_exceptions();

        if (currentUncaughtExceptions != uncaughtExceptions_)
        {
            functor_();
        }
    }

private:
    const int uncaughtExceptions_;
    const std::decay_t<FunctorType> functor_;
};

template<typename FunctorType>
auto makeScopeFailureGuard(FunctorType&& functor)
{
    return ScopeFailureGuard<std::decay_t<FunctorType>>(std::forward<FunctorType>(functor));
}

}

#define SCOPE_FAILURE(...)                                                                                             \
    const auto DANSANDU_JOURNEY_UNIQUE_NAME = dansandu::ballotin::scope::makeScopeFailureGuard(__VA_ARGS__)
