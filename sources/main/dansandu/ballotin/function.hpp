#pragma once

#include "dansandu/ballotin/exception.hpp"
#include "dansandu/ballotin/type_traits.hpp"

#include <concepts>
#include <type_traits>

namespace dansandu::ballotin::function
{

template<typename T>
class Function;

template<typename T>
struct IsFunctionTemplate
{
    static constexpr auto value = false;
};

template<typename T>
struct IsFunctionTemplate<Function<T>>
{
    static constexpr auto value = true;
};

template<typename Return, typename... Arguments>
class Function<Return(Arguments...)>
{
public:
    using FunctionPointer = Return (*)(Arguments... arguments);

    Function() : object_{nullptr}, dispatcher_{nullptr}, functionPointer_{nullptr}, invoker_{nullptr}
    {
    }

    Function(const FunctionPointer functionPointer)
        : object_{nullptr}, dispatcher_{nullptr}, functionPointer_{functionPointer}, invoker_{functionPointerInvoker}
    {
    }

    template<typename T>
    Function(T&& object)
        requires !IsFunctionTemplate<std::decay_t<T>>::value && !std::is_pointer_v<std::decay_t<T>> &&
                     dansandu::ballotin::type_traits::Invokable<T, Return, Arguments...>
        : object_{new std::decay_t<T>{std::forward<T>(object)}},
          dispatcher_{InvokableTraits<std::decay_t<T>>::dispatcher},
          functionPointer_{nullptr},
          invoker_{InvokableTraits<std::decay_t<T>>::invoker}
    {
    }

    Function(const Function& other)
        : object_{nullptr},
          dispatcher_{other.dispatcher_},
          functionPointer_{other.functionPointer_},
          invoker_{other.invoker_}
    {
        if (dispatcher_)
        {
            object_ = dispatcher_(Operation::copy, other.object_);
        }
    }

    Function(Function&& other) noexcept
        : object_{other.object_},
          dispatcher_{other.dispatcher_},
          functionPointer_{other.functionPointer_},
          invoker_{other.invoker_}
    {
        other.object_ = nullptr;
        other.dispatcher_ = nullptr;
        other.functionPointer_ = nullptr;
        other.invoker_ = nullptr;
    }

    Function& operator=(const Function& other)
    {
        if (this == &other)
        {
            return *this;
        }

        void* temporary = nullptr;

        if (other.dispatcher_)
        {
            temporary = other.dispatcher_(Operation::copy, other.object_);
        }

        if (dispatcher_)
        {
            dispatcher_(Operation::destruct, object_);
        }

        object_ = temporary;
        dispatcher_ = other.dispatcher_;
        functionPointer_ = other.functionPointer_;
        invoker_ = other.invoker_;

        return *this;
    }

    Function& operator=(Function&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        if (dispatcher_)
        {
            dispatcher_(Operation::destruct, object_);
        }

        object_ = other.object_;
        dispatcher_ = other.dispatcher_;
        functionPointer_ = other.functionPointer_;
        invoker_ = other.invoker_;

        other.object_ = nullptr;
        other.dispatcher_ = nullptr;
        other.functionPointer_ = nullptr;
        other.invoker_ = nullptr;

        return *this;
    }

    ~Function() noexcept
    {
        if (dispatcher_)
        {
            dispatcher_(Operation::destruct, object_);
        }
    }

    template<typename... ForwardedArguments>
    Return operator()(ForwardedArguments&&... arguments) const
    {
        if (invoker_)
        {
            return invoker_(object_, functionPointer_, std::forward<ForwardedArguments>(arguments)...);
        }
        else
        {
            THROW(std::logic_error, "Function doesn't not hold any invokable object");
        }
    }

    bool isEmpty() const
    {
        return invoker_ == nullptr;
    }

private:
    enum class Operation
    {
        copy,
        destruct,
    };

    using Invoker = Return (*)(void* const object, const FunctionPointer functionPointer, Arguments&&... arguments);
    using Dispatcher = void* (*)(const Operation operation, void* const object);

    static Return functionPointerInvoker(void* const, const FunctionPointer functionPointer, Arguments&&... arguments)
    {
        if constexpr (std::is_same_v<Return, void>)
        {
            functionPointer(std::forward<Arguments>(arguments)...);
        }
        else
        {
            return functionPointer(std::forward<Arguments>(arguments)...);
        }
    }

    template<typename T>
    struct InvokableTraits
    {
        using DecayedType = std::decay_t<T>;

        static Return invoker(void* const object, const FunctionPointer, Arguments&&... arguments)
        {
            const auto casted = static_cast<DecayedType*>(object);
            if constexpr (std::is_same_v<Return, void>)
            {
                casted->operator()(std::forward<Arguments>(arguments)...);
            }
            else
            {
                return casted->operator()(std::forward<Arguments>(arguments)...);
            }
        }

        static void* dispatcher(const Operation operation, void* const object)
        {
            switch (operation)
            {
            case Operation::copy:
                return new DecayedType{*static_cast<DecayedType*>(object)};
            case Operation::destruct:
                delete static_cast<DecayedType*>(object);
                return nullptr;
            default:
                THROW(std::logic_error, "Unknown operation");
            }
        }
    };

    void* object_;
    Dispatcher dispatcher_;
    FunctionPointer functionPointer_;
    Invoker invoker_;
};

}
