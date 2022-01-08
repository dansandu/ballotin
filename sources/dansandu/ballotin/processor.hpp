#pragma once

#include "dansandu/ballotin/type_traits.hpp"

#include <chrono>
#include <memory>

namespace dansandu::ballotin::processor
{

class PRALINE_EXPORT IProcess : private dansandu::ballotin::type_traits::Uncopyable,
                                private dansandu::ballotin::type_traits::Immovable
{
public:
    virtual ~IProcess()
    {
    }

    virtual void reset() = 0;

    virtual void tick() = 0;

    virtual bool done() const = 0;
};

class PRALINE_EXPORT Processor
{
public:
    Processor(std::unique_ptr<IProcess> process, const std::chrono::milliseconds interval);

    ~Processor();

    void resume();

    void pause();

    void reset();

    std::unique_ptr<IProcess> wait();

    std::unique_ptr<IProcess> yield();

private:
    std::unique_ptr<void, void (*)(void*)> implementation_;
};

}
