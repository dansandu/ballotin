#include "dansandu/ballotin/processor.hpp"

namespace dansandu::ballotin::processor
{

Processor::Processor(std::unique_ptr<IProcess> process) : process_{std::move(process)}
{
    if (process_)
    {
        thread_ = std::thread{&Processor::loop, this};
    }
}

void Processor::resume()
{
    if (process_)
    {
        {
            const auto lock = std::lock_guard{mutex_};
            queue_.push_back(Signal::resume);
        }
        conditionVariable_.notify_one();
    }
}

void Processor::pause()
{
    if (process_)
    {
        {
            const auto lock = std::lock_guard{mutex_};
            queue_.push_back(Signal::pause);
        }
        conditionVariable_.notify_one();
    }
}

void Processor::reset()
{
    if (process_)
    {
        {
            const auto lock = std::lock_guard{mutex_};
            queue_.push_back(Signal::reset);
        }
        conditionVariable_.notify_one();
    }
}

std::unique_ptr<IProcess> Processor::yield()
{
    if (process_)
    {
        {
            const auto lock = std::lock_guard{mutex_};
            queue_.push_back(Signal::yield);
        }
        conditionVariable_.notify_one();
        thread_.join();
        return std::move(process_);
    }
    return {};
}

std::unique_ptr<IProcess> Processor::finishAndYield()
{
    if (process_)
    {
        {
            const auto lock = std::lock_guard{mutex_};
            queue_.push_back(Signal::finishAndYield);
        }
        conditionVariable_.notify_one();
        thread_.join();
        return std::move(process_);
    }
    return {};
}

Processor::~Processor()
{
    yield();
}

void Processor::loop()
{
    auto paused = true;
    auto done = false;
    auto finishing = false;

    while (!done | !finishing)
    {
        if (!finishing)
        {
            auto reset = false;

            {
                auto lock = std::unique_lock{mutex_};
                conditionVariable_.wait(lock, [paused, this] { return !queue_.empty() | !paused; });

                for (auto index = 0U; index < queue_.size(); ++index)
                {
                    switch (queue_[index])
                    {
                    case Signal::resume:
                        paused = done;
                        break;
                    case Signal::pause:
                        paused = true;
                        break;
                    case Signal::reset:
                        paused = true;
                        done = false;
                        reset = true;
                        break;
                    case Signal::yield:
                        return;
                    case Signal::finishAndYield:
                        paused = done;
                        finishing = true;
                        index = queue_.size();
                        break;
                    }
                }

                queue_.clear();
            }

            if (reset)
            {
                process_->reset();
            }
        }

        if (!paused & !(done || (done = process_->done())))
        {
            paused = done;
            process_->tick();
        }
    }
}

}
