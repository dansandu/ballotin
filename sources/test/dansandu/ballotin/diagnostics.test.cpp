#include "dansandu/ballotin/diagnostics.hpp"
#include "dansandu/radiance/radiance.hpp"

using dansandu::ballotin::diagnostics::TrackedObject;
using dansandu::ballotin::diagnostics::TrackedObjectSession;

TEST_CASE("diagnostics")
{
    auto session = TrackedObjectSession{};

    SECTION("construction from session")
    {
        {
            const auto object = TrackedObject{session};

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

    SECTION("copy construction")
    {
        {
            const auto original = TrackedObject{session};

            const auto copy = original;

            REQUIRE(session.getCreatedInstances() == 2);

            REQUIRE(session.getCopyConstructorCalls() == 1);

            REQUIRE(session.getMoveConstructorCalls() == 0);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 0);
        }

        REQUIRE(session.getCreatedInstances() == 2);

        REQUIRE(session.getCopyConstructorCalls() == 1);

        REQUIRE(session.getMoveConstructorCalls() == 0);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 2);
    }

    SECTION("move construction")
    {
        {
            auto original = TrackedObject{session};

            const auto moved = std::move(original);

            REQUIRE(session.getCreatedInstances() == 2);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 1);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 0);
        }

        REQUIRE(session.getCreatedInstances() == 2);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 1);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 2);
    }

    SECTION("copy assignment")
    {
        {
            auto original = TrackedObject{session};

            auto copyAssignee = TrackedObject{session};

            copyAssignee = original;

            REQUIRE(session.getCreatedInstances() == 2);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 0);

            REQUIRE(session.getCopyAssignmentCalls() == 1);

            REQUIRE(session.getMoveAssignmentCalls() == 0);

            REQUIRE(session.getDestructorCalls() == 0);
        }

        REQUIRE(session.getCreatedInstances() == 2);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 0);

        REQUIRE(session.getCopyAssignmentCalls() == 1);

        REQUIRE(session.getMoveAssignmentCalls() == 0);

        REQUIRE(session.getDestructorCalls() == 2);
    }

    SECTION("move assignment")
    {
        {
            auto original = TrackedObject{session};

            auto copyAssignee = TrackedObject{session};

            copyAssignee = std::move(original);

            REQUIRE(session.getCreatedInstances() == 2);

            REQUIRE(session.getCopyConstructorCalls() == 0);

            REQUIRE(session.getMoveConstructorCalls() == 0);

            REQUIRE(session.getCopyAssignmentCalls() == 0);

            REQUIRE(session.getMoveAssignmentCalls() == 1);

            REQUIRE(session.getDestructorCalls() == 0);
        }

        REQUIRE(session.getCreatedInstances() == 2);

        REQUIRE(session.getCopyConstructorCalls() == 0);

        REQUIRE(session.getMoveConstructorCalls() == 0);

        REQUIRE(session.getCopyAssignmentCalls() == 0);

        REQUIRE(session.getMoveAssignmentCalls() == 1);

        REQUIRE(session.getDestructorCalls() == 2);
    }
}
