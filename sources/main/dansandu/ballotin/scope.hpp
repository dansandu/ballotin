#pragma once

#include "dansandu/journey/macro.hpp"

#include <exception>
#include <type_traits>

namespace dansandu::ballotin::scope
{

enum class ScopeGuardStrategy
{
    fireOnFailure,
    fireOnSuccess,
    fireOnExit,
};

template<ScopeGuardStrategy strategy, typename FunctorType>
class ScopeGuard
{
public:
    template<typename F>
    explicit ScopeGuard(F&& functor)
        : uncaughtExceptions_{std::uncaught_exceptions()}, functor_{std::forward<F>(functor)}
    {
    }

    ScopeGuard() = delete;
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard(ScopeGuard&&) noexcept = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
    ScopeGuard& operator=(ScopeGuard&&) noexcept = delete;

    ~ScopeGuard() noexcept
    {
        if constexpr (strategy == ScopeGuardStrategy::fireOnFailure)
        {
            const auto currentUncaughtExceptions = std::uncaught_exceptions();

            if (currentUncaughtExceptions != uncaughtExceptions_)
            {
                functor_();
            }
        }
        else if constexpr (strategy == ScopeGuardStrategy::fireOnSuccess)
        {
            const auto currentUncaughtExceptions = std::uncaught_exceptions();

            if (currentUncaughtExceptions == uncaughtExceptions_)
            {
                functor_();
            }
        }
        else if constexpr (strategy == ScopeGuardStrategy::fireOnExit)
        {
            functor_();
        }
        else
        {
            static_assert(true, "Unknown scope guard strategy");
        }
    }

private:
    const int uncaughtExceptions_;
    const std::decay_t<FunctorType> functor_;
};

template<typename FunctorType>
auto makeScopeFailureGuard(FunctorType&& functor)
{
    return ScopeGuard<ScopeGuardStrategy::fireOnFailure, std::decay_t<FunctorType>>(std::forward<FunctorType>(functor));
}

template<typename FunctorType>
auto makeScopeSuccessGuard(FunctorType&& functor)
{
    return ScopeGuard<ScopeGuardStrategy::fireOnSuccess, std::decay_t<FunctorType>>(std::forward<FunctorType>(functor));
}

template<typename FunctorType>
auto makeScopeExitGuard(FunctorType&& functor)
{
    return ScopeGuard<ScopeGuardStrategy::fireOnExit, std::decay_t<FunctorType>>(std::forward<FunctorType>(functor));
}

}

#define SCOPE_FAILURE(...)                                                                                             \
    const auto DANSANDU_JOURNEY_UNIQUE_NAME = ::dansandu::ballotin::scope::makeScopeFailureGuard(__VA_ARGS__)

#define SCOPE_SUCCESS(...)                                                                                             \
    const auto DANSANDU_JOURNEY_UNIQUE_NAME = ::dansandu::ballotin::scope::makeScopeSuccessGuard(__VA_ARGS__)

#define SCOPE_EXIT(...)                                                                                                \
    const auto DANSANDU_JOURNEY_UNIQUE_NAME = ::dansandu::ballotin::scope::makeScopeExitGuard(__VA_ARGS__)
