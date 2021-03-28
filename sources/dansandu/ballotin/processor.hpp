#pragma once

#include "dansandu/ballotin/type_traits.hpp"

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

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

class PRALINE_EXPORT Processor : private dansandu::ballotin::type_traits::Uncopyable,
                                 private dansandu::ballotin::type_traits::Immovable
{
public:
    Processor(std::unique_ptr<IProcess> process);

    ~Processor();

    void resume();

    void pause();

    void reset();

    std::unique_ptr<IProcess> yield();

    std::unique_ptr<IProcess> finishAndYield();

private:
    void loop();

    enum class Signal
    {
        resume,
        pause,
        reset,
        yield,
        finishAndYield
    };

    std::unique_ptr<IProcess> process_;
    std::mutex mutex_;
    std::condition_variable conditionVariable_;
    std::vector<Signal> queue_;
    std::thread thread_;
};

}
