#include "dansandu/ballotin/function.hpp"
#include "dansandu/ballotin/diagnostics.hpp"
#include "dansandu/radiance/radiance.hpp"

#include <stdexcept>
#include <type_traits>

using dansandu::ballotin::diagnostics::TrackedObject;
using dansandu::ballotin::diagnostics::TrackedObjectSession;
using dansandu::ballotin::function::Function;
using dansandu::ballotin::function::UniqueFunction;

namespace
{

#if defined(__clang__)
constexpr auto msvcCorrection = 0;
#elif defined(__GNUC__)
constexpr auto msvcCorrection = 0;
#elif defined(_MSC_VER)
constexpr auto msvcCorrection = 1;
#elif
#error "Unknown compiler"
#endif

void rawFunction(TrackedObject&)
{
}

}

TEST_CASE("function")
{
    SECTION("empty function")
    {
        Function<void()> function;

        REQUIRE(function.isEmpty());

        REQUIRE_THROW(std::logic_error, function());
    }

    SECTION("construction from function pointer")
    {
        auto session = TrackedObjectSession{};

        {
            auto parameter = TrackedObject{session};

            Function<void(TrackedObject&)> function = rawFunction;

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
            Function<void()> function = [member = TrackedObject{session}]() {};

            REQUIRE(!function.isEmpty());

            REQUIRE(session.getCreatedInstances() == 2 + msvcCorrection);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 1 + msvcCorrection);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1 + msvcCorrection);

            function();

            REQUIRE(session.getCreatedInstances() == 2 + msvcCorrection);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 1 + msvcCorrection);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1 + msvcCorrection);
        }

        REQUIRE(session.getCreatedInstances() == 2 + msvcCorrection);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 1 + msvcCorrection);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 2 + msvcCorrection);
    }

    SECTION("construction from mutable lambda object")
    {
        auto session = TrackedObjectSession{};

        {
            Function<void()> function = [member = TrackedObject{session}]() mutable {};

            REQUIRE(!function.isEmpty());

            REQUIRE(session.getCreatedInstances() == 2 + msvcCorrection);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 1 + msvcCorrection);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1 + msvcCorrection);

            function();

            REQUIRE(session.getCreatedInstances() == 2 + msvcCorrection);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 1 + msvcCorrection);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1 + msvcCorrection);
        }

        REQUIRE(session.getCreatedInstances() == 2 + msvcCorrection);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 1 + msvcCorrection);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 2 + msvcCorrection);
    }

    SECTION("construction from invokable type")
    {
        auto session = TrackedObjectSession{};

        struct InvokableType
        {
            explicit InvokableType(TrackedObjectSession& session) : member{session}
            {
            }

            int operator()() const
            {
                return 71;
            }

            TrackedObject member;
        };

        {
            Function<int()> function = InvokableType{session};

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

    SECTION("copy construction")
    {
        auto session = TrackedObjectSession{};

        {
            Function<int()> original = [object = TrackedObject{session}]() { return 13; };

            REQUIRE(!original.isEmpty());

            REQUIRE(session.getCreatedInstances() == 2 + msvcCorrection);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 1 + msvcCorrection);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1 + msvcCorrection);

            Function<int()> copy = original;

            REQUIRE(!original.isEmpty());

            REQUIRE(!copy.isEmpty());

            REQUIRE(session.getCreatedInstances() == 3 + msvcCorrection);

            REQUIRE(session.getCopyConstructorCalls() == 1);

            REQUIRE(session.getMoveConstructorCalls() == 1 + msvcCorrection);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1 + msvcCorrection);

            REQUIRE(original() == 13);

            REQUIRE(copy() == 13);
        }

        REQUIRE(session.getCreatedInstances() == 3 + msvcCorrection);

        REQUIRE(session.getCopyConstructorCalls() == 1);

        REQUIRE(session.getMoveConstructorCalls() == 1 + msvcCorrection);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 3 + msvcCorrection);
    }

    SECTION("move construction")
    {
        auto session = TrackedObjectSession{};

        {
            Function<int()> original = [object = TrackedObject{session}]() { return 17; };

            REQUIRE(!original.isEmpty());

            REQUIRE(session.getCreatedInstances() == 2 + msvcCorrection);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 1 + msvcCorrection);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1 + msvcCorrection);

            Function<int()> moved = std::move(original);

            REQUIRE(original.isEmpty());

            REQUIRE(!moved.isEmpty());

            REQUIRE(session.getCreatedInstances() == 2 + msvcCorrection);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 1 + msvcCorrection);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 1 + msvcCorrection);

            REQUIRE_THROW(std::logic_error, original());

            REQUIRE(moved() == 17);
        }

        REQUIRE(session.getCreatedInstances() == 2 + msvcCorrection);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 1 + msvcCorrection);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 2 + msvcCorrection);
    }

    SECTION("copy assignment")
    {
        auto session = TrackedObjectSession{};

        {
            Function<int()> original = [object = TrackedObject{session}]() { return 17; };

            Function<int()> assignee = [object = TrackedObject{session}]() { return 31; };

            REQUIRE(session.getCreatedInstances() == 4 + 2 * msvcCorrection);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 2 + 2 * msvcCorrection);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 2 + 2 * msvcCorrection);

            REQUIRE(!original.isEmpty());

            REQUIRE(!assignee.isEmpty());

            assignee = original;

            REQUIRE(session.getCreatedInstances() == 5 + 2 * msvcCorrection);

            REQUIRE(session.getCopyConstructorCalls() == 1);

            REQUIRE(session.getMoveConstructorCalls() == 2 + 2 * msvcCorrection);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 3 + 2 * msvcCorrection);

            REQUIRE(!original.isEmpty());

            REQUIRE(!assignee.isEmpty());

            REQUIRE(assignee() == 17);

            REQUIRE(original() == 17);
        }

        REQUIRE(session.getCreatedInstances() == 5 + 2 * msvcCorrection);

        REQUIRE(session.getCopyConstructorCalls() == 1);

        REQUIRE(session.getMoveConstructorCalls() == 2 + 2 * msvcCorrection);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 5 + 2 * msvcCorrection);
    }

    SECTION("move assignment")
    {
        auto session = TrackedObjectSession{};

        {
            Function<int()> original = [object = TrackedObject{session}]() { return 17; };

            Function<int()> assignee = [object = TrackedObject{session}]() { return 31; };

            REQUIRE(session.getCreatedInstances() == 4 + 2 * msvcCorrection);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 2 + 2 * msvcCorrection);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 2 + 2 * msvcCorrection);

            REQUIRE(!original.isEmpty());

            REQUIRE(!assignee.isEmpty());

            assignee = std::move(original);

            REQUIRE(session.getCreatedInstances() == 4 + 2 * msvcCorrection);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 2 + 2 * msvcCorrection);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 3 + 2 * msvcCorrection);

            REQUIRE(original.isEmpty());

            REQUIRE(!assignee.isEmpty());

            REQUIRE(assignee() == 17);

            REQUIRE_THROW(std::logic_error, original());
        }

        REQUIRE(session.getCreatedInstances() == 4 + 2 * msvcCorrection);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 2 + 2 * msvcCorrection);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 4 + 2 * msvcCorrection);
    }

    SECTION("argument passing by mutable left value reference")
    {
        auto session = TrackedObjectSession{};

        {
            auto parameter = TrackedObject{session};

            Function<void(TrackedObject&)> function = [](TrackedObject&) {};

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

            Function<void(const TrackedObject&)> function = [](const TrackedObject&) {};

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
            Function<void(TrackedObject&&)> function = [](TrackedObject&&) {};

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
            Function<void(const TrackedObject&&)> function = [](const TrackedObject&&) {};

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
            Function<void(TrackedObject)> function = [](TrackedObject) {};

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
            REQUIRE(std::is_assignable_v<Function<void(int)>, void (*)(int)>);

            REQUIRE(std::is_assignable_v<Function<void(int&)>, void (*)(int&)>);

            REQUIRE(std::is_assignable_v<Function<void(const int)>, void (*)(int)>);

            REQUIRE(std::is_assignable_v<Function<void(int)>, void (*)(const int)>);
        }

        SECTION("negative")
        {
            REQUIRE(!std::is_assignable_v<Function<void()>, UniqueFunction<void()>>);

            REQUIRE(!std::is_assignable_v<Function<void(int&)>, void (*)(int)>);

            REQUIRE(!std::is_assignable_v<Function<void(int)>, void (*)(int&)>);

            REQUIRE(!std::is_assignable_v<Function<void(int&&)>, void (*)(int&)>);

            REQUIRE(!std::is_assignable_v<Function<void(const int&&)>, void (*)(int&&)>);

            REQUIRE(!std::is_assignable_v<Function<void(const int&)>, void (*)(int&)>);

            REQUIRE(!std::is_assignable_v<Function<void(int&&)>, void (*)(const int&&)>);

            REQUIRE(!std::is_assignable_v<Function<void(int&)>, void (*)(const int&)>);

            REQUIRE(!std::is_assignable_v<Function<double()>, int (*)()>);

            REQUIRE(!std::is_assignable_v<Function<void(double)>, void (*)(int)>);
        }
    }
}
