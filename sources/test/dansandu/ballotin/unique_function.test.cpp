#include "dansandu/ballotin/diagnostics.hpp"
#include "dansandu/ballotin/function.hpp"
#include "dansandu/radiance/radiance.hpp"

#include <stdexcept>
#include <type_traits>

using dansandu::ballotin::diagnostics::TrackedObject;
using dansandu::ballotin::diagnostics::TrackedObjectSession;
using dansandu::ballotin::function::Function;
using dansandu::ballotin::function::UniqueFunction;

namespace
{

void rawFunction(TrackedObject&)
{
}

}

TEST_CASE("unique_function")
{
    SECTION("empty function")
    {
        UniqueFunction<void()> function;

        REQUIRE(function.isEmpty());

        REQUIRE_THROW(std::logic_error, function());
    }

    SECTION("construction from function pointer")
    {
        auto session = TrackedObjectSession{};

        {
            auto parameter = TrackedObject{session};

            UniqueFunction<void(TrackedObject&)> function = rawFunction;

            REQUIRE(!function.isEmpty());

            REQUIRE(session.getCreatedInstances() == 1);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 0);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 0);

            function(parameter);

            REQUIRE(session.getCreatedInstances() == 1);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 0);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 0);
        }

        REQUIRE(session.getCreatedInstances() == 1);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 0);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 1);
    }

    SECTION("construction from lambda object")
    {
        auto session = TrackedObjectSession{};

        {
            UniqueFunction<void()> function = [member = TrackedObject{session}]() {};

            REQUIRE(!function.isEmpty());

            REQUIRE(session.getCreatedInstances() == 2);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 1);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1);

            function();

            REQUIRE(session.getCreatedInstances() == 2);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 1);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1);
        }

        REQUIRE(session.getCreatedInstances() == 2);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 1);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 2);
    }

    SECTION("construction from mutable lambda object")
    {
        auto session = TrackedObjectSession{};

        {
            UniqueFunction<void()> function = [member = TrackedObject{session}]() mutable {};

            REQUIRE(!function.isEmpty());

            REQUIRE(session.getCreatedInstances() == 2);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 1);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1);

            function();

            REQUIRE(session.getCreatedInstances() == 2);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 1);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1);
        }

        REQUIRE(session.getCreatedInstances() == 2);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 1);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 2);
    }

    SECTION("construction from uncopyable invokable type")
    {
        auto session = TrackedObjectSession{};

        struct InvokableType
        {
            explicit InvokableType(TrackedObjectSession& session) : member{session}
            {
            }

            InvokableType(const InvokableType&) = delete;
            InvokableType(InvokableType&&) noexcept = default;

            InvokableType& operator=(const InvokableType&) = delete;
            InvokableType& operator=(InvokableType&&) noexcept = default;

            int operator()() const
            {
                return 71;
            }

            TrackedObject member;
        };

        {
            UniqueFunction<int()> function = InvokableType{session};

            REQUIRE(!function.isEmpty());

            REQUIRE(session.getCreatedInstances() == 2);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 1);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1);

            REQUIRE(function() == 71);

            REQUIRE(session.getCreatedInstances() == 2);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 1);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1);
        }

        REQUIRE(session.getCreatedInstances() == 2);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 1);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 2);
    }

    SECTION("move construction")
    {
        auto session = TrackedObjectSession{};

        {
            UniqueFunction<int()> original = [object = TrackedObject{session}]() { return 17; };

            REQUIRE(!original.isEmpty());

            REQUIRE(session.getCreatedInstances() == 2);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 1);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1);

            UniqueFunction<int()> moved = std::move(original);

            REQUIRE(original.isEmpty());

            REQUIRE(!moved.isEmpty());

            REQUIRE(session.getCreatedInstances() == 2);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 1);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1);

            REQUIRE_THROW(std::logic_error, original());

            REQUIRE(moved() == 17);
        }

        REQUIRE(session.getCreatedInstances() == 2);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 1);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 2);
    }

    SECTION("move construction from Function")
    {
        auto session = TrackedObjectSession{};

        {
            Function<int()> original = [object = TrackedObject{session}]() { return 121; };

            UniqueFunction<int()> function = std::move(original);

            REQUIRE(!function.isEmpty());

            REQUIRE(session.getCreatedInstances() == 2);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 1);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1);

            REQUIRE(function() == 121);

            REQUIRE_THROW(std::logic_error, original());
        }

        REQUIRE(session.getCreatedInstances() == 2);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 1);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 2);
    }

    SECTION("move assignment")
    {
        auto session = TrackedObjectSession{};

        {
            UniqueFunction<int()> original = [object = TrackedObject{session}]() { return 17; };

            UniqueFunction<int()> assignee = [object = TrackedObject{session}]() { return 31; };

            REQUIRE(session.getCreatedInstances() == 4);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 2);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 2);

            REQUIRE(!original.isEmpty());

            REQUIRE(!assignee.isEmpty());

            assignee = std::move(original);

            REQUIRE(session.getCreatedInstances() == 4);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 2);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 3);

            REQUIRE(original.isEmpty());

            REQUIRE(!assignee.isEmpty());

            REQUIRE(assignee() == 17);

            REQUIRE_THROW(std::logic_error, original());
        }

        REQUIRE(session.getCreatedInstances() == 4);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 2);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 4);
    }

    SECTION("argument passing by mutable left value reference")
    {
        auto session = TrackedObjectSession{};

        {
            auto parameter = TrackedObject{session};

            UniqueFunction<void(TrackedObject&)> function = [](TrackedObject&) {};

            function(parameter);

            REQUIRE(session.getCreatedInstances() == 1);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 0);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 0);
        }

        REQUIRE(session.getCreatedInstances() == 1);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 0);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 1);
    }

    SECTION("argument passing by immutable left value reference")
    {
        auto session = TrackedObjectSession{};

        {
            const auto parameter = TrackedObject{session};

            UniqueFunction<void(const TrackedObject&)> function = [](const TrackedObject&) {};

            function(parameter);

            REQUIRE(session.getCreatedInstances() == 1);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 0);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 0);
        }

        REQUIRE(session.getCreatedInstances() == 1);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 0);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 1);
    }

    SECTION("argument passing by mutable right value reference")
    {
        auto session = TrackedObjectSession{};

        {
            UniqueFunction<void(TrackedObject&&)> function = [](TrackedObject&&) {};

            function(TrackedObject{session});

            REQUIRE(session.getCreatedInstances() == 1);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 0);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1);
        }

        REQUIRE(session.getCreatedInstances() == 1);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 0);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 1);
    }

    SECTION("argument passing by immutable right value reference")
    {
        auto session = TrackedObjectSession{};

        {
            UniqueFunction<void(const TrackedObject&&)> function = [](const TrackedObject&&) {};

            function(TrackedObject{session});

            REQUIRE(session.getCreatedInstances() == 1);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 0);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1);
        }

        REQUIRE(session.getCreatedInstances() == 1);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 0);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 1);
    }

    SECTION("argument passing by value")
    {
        auto session = TrackedObjectSession{};

        {
            UniqueFunction<void(TrackedObject)> function = [](TrackedObject) {};

            function(TrackedObject{session});

            REQUIRE(session.getCreatedInstances() == 3);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 2);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 3);
        }

        REQUIRE(session.getCreatedInstances() == 3);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 2);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 3);
    }

    SECTION("conversion")
    {
        SECTION("positive")
        {
            REQUIRE(std::is_assignable_v<UniqueFunction<void()>, Function<void()>>);

            REQUIRE(std::is_assignable_v<UniqueFunction<void(int)>, void (*)(int)>);

            REQUIRE(std::is_assignable_v<UniqueFunction<void(int&)>, void (*)(int&)>);

            REQUIRE(std::is_assignable_v<UniqueFunction<void(const int)>, void (*)(int)>);

            REQUIRE(std::is_assignable_v<UniqueFunction<void(int)>, void (*)(const int)>);
        }

        SECTION("negative")
        {
            REQUIRE(!std::is_assignable_v<UniqueFunction<void(int&)>, void (*)(int)>);

            REQUIRE(!std::is_assignable_v<UniqueFunction<void(int)>, void (*)(int&)>);

            REQUIRE(!std::is_assignable_v<UniqueFunction<void(int&&)>, void (*)(int&)>);

            REQUIRE(!std::is_assignable_v<UniqueFunction<void(const int&&)>, void (*)(int&&)>);

            REQUIRE(!std::is_assignable_v<UniqueFunction<void(const int&)>, void (*)(int&)>);

            REQUIRE(!std::is_assignable_v<UniqueFunction<void(int&&)>, void (*)(const int&&)>);

            REQUIRE(!std::is_assignable_v<UniqueFunction<void(int&)>, void (*)(const int&)>);

            REQUIRE(!std::is_assignable_v<UniqueFunction<double()>, int (*)()>);

            REQUIRE(!std::is_assignable_v<UniqueFunction<void(double)>, void (*)(int)>);
        }
    }
}
