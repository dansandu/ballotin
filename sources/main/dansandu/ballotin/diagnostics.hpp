#pragma once

namespace dansandu::ballotin::diagnostics
{

class TrackedObjectSession
{
public:
    friend class TrackedObject;

    TrackedObjectSession()
        : createdInstances_{0},
          copyConstructorCalls_{0},
          moveConstructorCalls_{0},
          copyAssignmentCalls_{0},
          moveAssignmentCalls_{0},
          destructorCalls_{0}
    {
    }

    TrackedObjectSession(const TrackedObjectSession&) = delete;
    TrackedObjectSession(TrackedObjectSession&&) noexcept = delete;
    TrackedObjectSession& operator=(const TrackedObjectSession&) = delete;
    TrackedObjectSession& operator=(TrackedObjectSession&&) noexcept = delete;

    int getCreatedInstances() const
    {
        return createdInstances_;
    }

    int getCopyConstructorCalls() const
    {
        return copyConstructorCalls_;
    }

    int getMoveConstructorCalls() const
    {
        return moveConstructorCalls_;
    }

    int getCopyAssignmentCalls() const
    {
        return copyAssignmentCalls_;
    }

    int getMoveAssignmentCalls() const
    {
        return moveAssignmentCalls_;
    }

    int getDestructorCalls() const
    {
        return destructorCalls_;
    }

private:
    int createdInstances_;
    int copyConstructorCalls_;
    int moveConstructorCalls_;
    int copyAssignmentCalls_;
    int moveAssignmentCalls_;
    int destructorCalls_;
};

class TrackedObject
{
public:
    explicit TrackedObject(TrackedObjectSession& session) : session_{&session}
    {
        ++session_->createdInstances_;
    }

    TrackedObject(const TrackedObject& other) : session_{other.session_}
    {
        ++session_->createdInstances_;
        ++session_->copyConstructorCalls_;
    }

    TrackedObject(TrackedObject&& other) noexcept : session_{other.session_}
    {
        ++session_->createdInstances_;
        ++session_->moveConstructorCalls_;
    }

    TrackedObject& operator=(const TrackedObject& other)
    {
        session_ = other.session_;
        ++session_->copyAssignmentCalls_;
        return *this;
    }

    TrackedObject& operator=(TrackedObject&& other) noexcept
    {
        session_ = other.session_;
        ++session_->moveAssignmentCalls_;
        return *this;
    }

    ~TrackedObject() noexcept
    {
        ++session_->destructorCalls_;
    }

private:
    TrackedObjectSession* session_;
};

}
