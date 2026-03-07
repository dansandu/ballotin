#pragma once

#include "dansandu/ballotin/exception.hpp"
#include "dansandu/ballotin/type_traits.hpp"

#include <concepts>
#include <type_traits>

namespace dansandu::ballotin::function
{

template<bool Copyable, typename T>
class FunctionImplementation;

template<typename T>
struct IsFunctionTemplate
{
    static constexpr auto value = false;
};

template<typename T>
struct IsFunctionTemplate<FunctionImplementation<true, T>>
{
    static constexpr auto value = true;
};

template<typename T>
struct IsFunctionTemplate<FunctionImplementation<false, T>>
{
    static constexpr auto value = true;
};

template<bool Copyable, typename Return, typename... Arguments>
class FunctionImplementation<Copyable, Return(Arguments...)>
{
public:
    using FunctionPointer = Return (*)(Arguments... arguments);

    friend class FunctionImplementation<!Copyable, Return(Arguments...)>;

    FunctionImplementation() : object_{nullptr}, dispatcher_{nullptr}, functionPointer_{nullptr}, invoker_{nullptr}
    {
    }

    FunctionImplementation(const FunctionPointer functionPointer)
        : object_{nullptr}, dispatcher_{nullptr}, functionPointer_{functionPointer}, invoker_{functionPointerInvoker}
    {
    }

    template<typename T>
    FunctionImplementation(T&& object)
        requires !IsFunctionTemplate<std::decay_t<T>>::value && !std::is_pointer_v<std::decay_t<T>> &&
                     dansandu::ballotin::type_traits::Invokable<T, Return, Arguments...>
        : object_{new std::decay_t<T>{std::forward<T>(object)}},
          dispatcher_{InvokableTraits<std::decay_t<T>>::dispatcher},
          functionPointer_{nullptr},
          invoker_{InvokableTraits<std::decay_t<T>>::invoker}
    {
    }

    FunctionImplementation(const FunctionImplementation& other)
        requires Copyable
        : object_{nullptr},
          dispatcher_{other.dispatcher_},
          functionPointer_{other.functionPointer_},
          invoker_{other.invoker_}
    {
        if (dispatcher_)
        {
            object_ = dispatcher_(copyOperation, other.object_);
        }
    }

    template<bool OtherCopyable>
    FunctionImplementation(FunctionImplementation<OtherCopyable, Return(Arguments...)>&& other) noexcept
        requires(!Copyable || OtherCopyable)
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

    FunctionImplementation& operator=(const FunctionImplementation& other)
        requires Copyable
    {
        if (this == &other)
        {
            return *this;
        }

        void* temporary = nullptr;

        if (other.dispatcher_)
        {
            temporary = other.dispatcher_(copyOperation, other.object_);
        }

        if (dispatcher_)
        {
            dispatcher_(destructOperation, object_);
        }

        object_ = temporary;
        dispatcher_ = other.dispatcher_;
        functionPointer_ = other.functionPointer_;
        invoker_ = other.invoker_;

        return *this;
    }

    FunctionImplementation& operator=(FunctionImplementation&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        if (dispatcher_)
        {
            dispatcher_(destructOperation, object_);
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

    ~FunctionImplementation() noexcept
    {
        if (dispatcher_)
        {
            dispatcher_(destructOperation, object_);
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
            THROW(std::logic_error, "FunctionImplementation doesn't not hold any invokable object");
        }
    }

    bool isEmpty() const
    {
        return invoker_ == nullptr;
    }

private:
    using Invoker = Return (*)(void* const object, const FunctionPointer functionPointer, Arguments... arguments);
    using Dispatcher = void* (*)(const bool operation, void* const object);

    static Return functionPointerInvoker(void* const, const FunctionPointer functionPointer, Arguments... arguments)
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

        static Return invoker(void* const object, const FunctionPointer, Arguments... arguments)
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

        static void* dispatcher(const bool operation, void* const object)
        {
            if constexpr (Copyable)
            {
                if (operation)
                {
                    return new DecayedType{*static_cast<DecayedType*>(object)};
                }
                else
                {
                    delete static_cast<DecayedType*>(object);
                    return nullptr;
                }
            }
            else
            {
                delete static_cast<DecayedType*>(object);
                return nullptr;
            }
        }
    };

    static constexpr auto copyOperation = true;
    static constexpr auto destructOperation = false;

    void* object_;
    Dispatcher dispatcher_;
    FunctionPointer functionPointer_;
    Invoker invoker_;
};

template<typename T>
using Function = FunctionImplementation<true, T>;

template<typename T>
using UniqueFunction = FunctionImplementation<false, T>;

}
