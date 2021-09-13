#include "dansandu/ballotin/processor.hpp"
#include "catchorg/catch/catch.hpp"
#include "dansandu/ballotin/exception.hpp"

#include <thread>
#include <vector>

using dansandu::ballotin::processor::IProcess;
using dansandu::ballotin::processor::Processor;

class CountIntegersProcess : public IProcess
{
public:
    CountIntegersProcess() : integer_{0}
    {
    }

    void reset() override
    {
        integer_ = 0;
    }

    void tick() override
    {
        if (done())
        {
            THROW(std::runtime_error, "processor should stop calling tick after exiting with false unless reset");
        }

        generated_.push_back(integer_++);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    bool done() const override
    {
        return integer_ >= 5;
    }

    const std::vector<int>& generated() const
    {
        return generated_;
    }

private:
    int integer_;
    std::vector<int> generated_;
};

static void sleep(int duration = 250)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(duration));
}

static auto cast(std::unique_ptr<IProcess> process)
{
    return std::unique_ptr<CountIntegersProcess>{dynamic_cast<CountIntegersProcess*>(process.release())};
}

using vec = std::vector<int>;

TEST_CASE("Processor", "[!mayfail]")
{
    auto processor = Processor{std::make_unique<CountIntegersProcess>()};

    SECTION("yield")
    {
        sleep();

        auto process = cast(processor.yield());

        REQUIRE(process->generated() == vec{});

        SECTION("then yield")
        {
            auto secondProcess = cast(processor.yield());

            REQUIRE(secondProcess == nullptr);
        }

        SECTION("then finish and yield")
        {
            auto secondProcess = cast(processor.finishAndYield());

            REQUIRE(secondProcess == nullptr);
        }
    }

    SECTION("finish and yield")
    {
        sleep();

        auto process = cast(processor.finishAndYield());

        REQUIRE(process->generated() == vec{0, 1, 2, 3, 4});

        SECTION("then yield")
        {
            auto secondProcess = cast(processor.yield());

            REQUIRE(secondProcess == nullptr);
        }

        SECTION("then finish and yield")
        {
            auto secondProcess = cast(processor.finishAndYield());

            REQUIRE(secondProcess == nullptr);
        }
    }

    SECTION("pause then yield")
    {
        processor.pause();

        sleep();

        auto process = cast(processor.yield());

        REQUIRE(process->generated() == vec{});
    }

    SECTION("reset then yield")
    {
        processor.reset();

        sleep();

        auto process = cast(processor.yield());

        REQUIRE(process->generated() == vec{});
    }

    SECTION("resume then yield")
    {
        processor.resume();

        sleep();

        auto process = cast(processor.yield());

        REQUIRE(process->generated() == vec{0, 1, 2});
    }

    SECTION("resume then pause then yield")
    {
        processor.resume();

        sleep();

        processor.pause();

        sleep();

        auto process = cast(processor.yield());

        REQUIRE(process->generated() == vec{0, 1, 2});
    }

    SECTION("resume then reset then yield")
    {
        processor.resume();

        sleep();

        processor.reset();

        sleep();

        auto process = cast(processor.yield());

        REQUIRE(process->generated() == vec{0, 1, 2});
    }

    SECTION("resume then reset then resume then yield")
    {
        processor.resume();

        sleep();

        processor.reset();

        sleep();

        processor.resume();

        sleep();

        auto process = cast(processor.yield());

        REQUIRE(process->generated() == vec{0, 1, 2, 0, 1, 2});
    }

    SECTION("resume then wait then finish")
    {
        processor.resume();

        sleep(800);

        auto process = cast(processor.yield());

        REQUIRE(process->generated() == vec{0, 1, 2, 3, 4});
    }

    SECTION("resume then wait then finish and yield")
    {
        processor.resume();

        sleep(800);

        auto process = cast(processor.finishAndYield());

        REQUIRE(process->generated() == vec{0, 1, 2, 3, 4});
    }

    SECTION("resume then wait then resume then wait then finish and yield")
    {
        processor.resume();

        sleep(800);

        processor.resume();

        sleep();

        auto process = cast(processor.finishAndYield());

        REQUIRE(process->generated() == vec{0, 1, 2, 3, 4});
    }
}
