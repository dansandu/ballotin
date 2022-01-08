#include "dansandu/ballotin/processor.hpp"

#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace dansandu::ballotin::processor
{

enum class Event
{
    resume,
    pause,
    reset,
    wait,
    yield,
};

struct Implementation
{
    static void deleter(void* pointer)
    {
        delete static_cast<Implementation*>(pointer);
    }

    Implementation(std::unique_ptr<IProcess> process, const std::chrono::milliseconds interval)
        : process_{std::move(process)}, interval_{interval}
    {
        if (process_)
        {
            thread_ = std::thread{&Implementation::loop, this};
        }
    }

    void loop()
    {
        auto paused = true;
        auto reset = false;
        auto done = false;
        auto wait = false;
        auto remaining = interval_;

        const auto handleEvents = [&]()
        {
            for (decltype(events_.size()) index = 0; index < events_.size(); ++index)
            {
                switch (events_[index])
                {
                case Event::resume:
                    // +--------+--------+-------------------------+
                    // |  wait  |  done  |  paused = wait & !done  |
                    // +--------+--------+-------------------------+
                    // |   0    |   0    |            0            |
                    // |   0    |   1    |            0            |
                    // |   1    |   0    |            1            |
                    // |   1    |   1    |            0            |
                    // +--------+--------+-------------------------+
                    paused = wait & !done;
                    break;
                case Event::pause:
                    // +--------+--------+-------------------------+
                    // |  wait  |  done  |  paused = !wait | done  |
                    // +--------+--------+-------------------------+
                    // |   0    |   0    |            1            |
                    // |   0    |   1    |            1            |
                    // |   1    |   0    |            0            |
                    // |   1    |   1    |            1            |
                    // +--------+--------+-------------------------+
                    paused = !wait | done;
                    break;
                case Event::reset:
                    // +--------+--------+-----------------+-------------------------+----------------------+
                    // |  wait  |  done  |  reset = !wait  |  paused = !wait | done  |  done = wait & done  |
                    // +--------+--------+-----------------+-------------------------+----------------------+
                    // |   0    |   0    |        1        |            1            |          0           |
                    // |   0    |   1    |        1        |            1            |          0           |
                    // |   1    |   0    |        0        |            0            |          0           |
                    // |   1    |   1    |        0        |            1            |          1           |
                    // +--------+--------+-----------------+-------------------------+----------------------+
                    reset = !wait;
                    paused = !wait | done;
                    done = wait & done;
                    break;
                case Event::wait:
                    // +--------+--------+-----------------+
                    // |  wait  |  done  |  paused = done  |
                    // +--------+--------+-----------------+
                    // |   0    |   0    |       n/a       |
                    // |   0    |   1    |       n/a       |
                    // |   1    |   0    |        0        |
                    // |   1    |   1    |        1        |
                    // +--------+--------+-----------------+
                    wait = true;
                    paused = done;
                    events_.clear();
                    break;
                case Event::yield:
                    return true;
                }
            }
            events_.clear();
            return false;
        };

        while (!done | !wait)
        {
            if (reset)
            {
                remaining = interval_;
                process_->reset();
                reset = false;
            }

            if (!paused & !(done || (done = process_->done())))
            {
                paused = done;
                process_->tick();
            }

            auto lock = std::unique_lock{mutex_};

            if (handleEvents())
            {
                return;
            }

            while (paused)
            {
                conditionVariable_.wait(lock);

                if (handleEvents())
                {
                    return;
                }
            }

            if (remaining > std::chrono::milliseconds::zero())
            {
                const auto timepoint = std::chrono::system_clock::now() + remaining;

                auto status = std::cv_status::no_timeout;

                while ((status != std::cv_status::timeout) & !paused)
                {
                    status = conditionVariable_.wait_until(lock, timepoint);
                    if (handleEvents())
                    {
                        return;
                    }
                }

                if (status == std::cv_status::timeout)
                {
                    remaining = interval_;
                }
                else
                {
                    remaining = std::chrono::duration_cast<std::chrono::milliseconds>(timepoint -
                                                                                      std::chrono::system_clock::now());
                }
            }
        }
    }

    std::thread thread_;
    std::condition_variable conditionVariable_;
    std::vector<Event> events_;
    std::unique_ptr<IProcess> process_;
    std::chrono::milliseconds interval_;
    std::mutex mutex_;
};

Processor::Processor(std::unique_ptr<IProcess> process, const std::chrono::milliseconds interval)
    : implementation_{new Implementation{std::move(process), interval}, &Implementation::deleter}
{
}

void Processor::resume()
{
    const auto casted = static_cast<Implementation*>(implementation_.get());
    if (casted->process_)
    {
        {
            const auto lock = std::lock_guard{casted->mutex_};
            casted->events_.push_back(Event::resume);
        }
        casted->conditionVariable_.notify_one();
    }
}

void Processor::pause()
{
    const auto casted = static_cast<Implementation*>(implementation_.get());
    if (casted->process_)
    {
        {
            const auto lock = std::lock_guard{casted->mutex_};
            casted->events_.push_back(Event::pause);
        }
        casted->conditionVariable_.notify_one();
    }
}

void Processor::reset()
{
    const auto casted = static_cast<Implementation*>(implementation_.get());
    if (casted->process_)
    {
        {
            const auto lock = std::lock_guard{casted->mutex_};
            casted->events_.push_back(Event::reset);
        }
        casted->conditionVariable_.notify_one();
    }
}

std::unique_ptr<IProcess> Processor::wait()
{
    const auto casted = static_cast<Implementation*>(implementation_.get());
    if (casted->process_)
    {
        {
            const auto lock = std::lock_guard{casted->mutex_};
            casted->events_.push_back(Event::wait);
        }
        casted->conditionVariable_.notify_one();
        casted->thread_.join();
    }
    return std::move(casted->process_);
}

std::unique_ptr<IProcess> Processor::yield()
{
    const auto casted = static_cast<Implementation*>(implementation_.get());
    if (casted->process_)
    {
        {
            const auto lock = std::lock_guard{casted->mutex_};
            casted->events_.push_back(Event::yield);
        }
        casted->conditionVariable_.notify_one();
        casted->thread_.join();
    }
    return std::move(casted->process_);
}

Processor::~Processor()
{
    yield();
}

}
