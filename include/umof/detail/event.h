#pragma once

#include <functional>

template<typename T>
struct is_callable_impl
{
private:
    typedef char(&yes)[1];
    typedef char(&no)[2];

    struct Fallback { void operator()(); };
    struct Derived : T, Fallback {};

    template<typename U, U> struct Check;

    template<typename>
    static yes test(...);

    template<typename C>
    static no test(Check<void (Fallback::*)(), &C::operator()>*);

public:
    static const bool value = sizeof(test<Derived>(0)) == sizeof(yes);
};

template<typename T>
struct is_method :
    std::conditional <
    std::is_member_function_pointer<T>::value
    || std::is_function<T>::value,
    std::true_type,
    std::false_type
    > ::type
{
};

template<typename T>
struct is_callable :
    std::conditional <
    std::is_class<T>::value,
    is_callable_impl<T>,
    is_method<T>
    > ::type
{
};

template<typename Signature, typename Method>
struct UMethodHelper
{
    using Args = MethodArguments<Signature>;
    static Method functor;

    inline static void call(const void *object, const void *ret, const void **stack)
    {
        Args::call(functor, object, ret, stack, unpack::indices_gen<Args::count>());
    }
};

template<typename Signature>
struct UMethodAdaptor
{
    using Args = MethodArguments<Signature>;

    template<typename Functor>
    static const MethodTable *table(Functor f)
    {
        UMethodHelper<Signature, Functor>::functor = f;
        static const MethodTable table = {
            "",
            &UMethodHelper<Signature, Functor>::call,
            MethodArguments<Signature>::count,
            MethodArguments<Signature>::types()
        };
        return &table;
    }
};

template<typename Signature, typename Method>
Method UMethodHelper<Signature, Method>::functor;

template<typename Signature, typename Functor>
struct UFunctorHelper
{
    using Args = MethodArguments<Signature>;
    static Functor functor;

    inline static void call(const void *object, const void *ret, const void **stack)
    {
        Args::lambda(functor, ret, stack, unpack::indices_gen<Args::count>());
    }
};

template<typename Signature, typename Functor>
Functor UFunctorHelper<Signature, Functor>::functor;

template<typename S>
struct UFunctorAdaptor
{
    using Signature = decltype(&S::operator());
    using Args = MethodArguments<Signature>;

    template<typename Functor>
    static const MethodTable *table(Functor f)
    {
        UFunctorHelper<Signature, Functor>::functor = f;
        static const MethodTable table = {
            "",
            &UFunctorHelper<Signature, Functor>::call,
            MethodArguments<Signature>::count,
            MethodArguments<Signature>::types()
        };
        return &table;
    }
};

template<typename Functor>
struct UAdaptor
{
    static_assert(is_callable<Functor>::value, "Argument should be callable");

    using IsObject = std::is_class<Functor>;
    using AdaptorType = typename std::conditional<
        IsObject::value,
        UFunctorAdaptor<Functor>,
        UMethodAdaptor<Functor>
    >::type;

    template<typename Object>
    static const MethodTable *table(Object o)
    {
        return AdaptorType::table(o);
    }
};
