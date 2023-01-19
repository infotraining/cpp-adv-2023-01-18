#include <array>
#include <catch2/catch_test_macros.hpp>

template <typename T>
struct Identity
{
    using type = T;
};

template <typename T, T v>
struct IntegralConstant
{
    static constexpr T value = v;
};

template <bool v>
using BoolConstant = IntegralConstant<bool, v>;

using TrueType = BoolConstant<true>;
using FalseType = BoolConstant<false>;

template <typename T>
void foo(T arg)
{
    static_assert(std::is_same_v<typename Identity<T>::type, int>);
}

TEST_CASE("type traits")
{
    static_assert(std::is_same_v<Identity<int>::type, int>);

    foo(3);

    static_assert(IntegralConstant<int, 3>::value == 3);

    static_assert(BoolConstant<true>::value == true);

    static_assert(TrueType::value == true);
    static_assert(FalseType::value == false);
}

////////////////////////////////////////////////////////////////
// RemoveRef - std::remove_reference

template <typename T>
struct RemoveRef
{
    using type = T;
};

template <typename T>
struct RemoveRef<T&>
{
    using type = T;
};

template <typename T>
struct RemoveRef<T&&>
{
    using type = T;
};

template <typename T>
using RemoveRef_t = typename RemoveRef<T>::type;

template <typename T>
auto need_for_trait(T&& item)
{
    // std::array<std::remove_const_t<std::remove_reference_t<T>>, 3> buffer{};

    static_assert(std::is_arithmetic_v<std::decay_t<T>>, "T must be a number type");
    std::array<std::decay_t<T>, 3> buffer{};

    buffer[0] = std::forward<T>(item);

    return buffer;
}

///////////////////////////////////////
// IsVoid

template <typename T>
struct IsVoid : FalseType
{ };
// {
//     static constexpr bool value = false;
// };

template <>
struct IsVoid<void> : TrueType
{ };
// {
//     static constexpr bool value = true;
// };

template <typename T>
constexpr bool IsVoid_v = IsVoid<T>::value;

///////////////////////////////////////
// IsPointer

template <typename T>
struct IsPointer : FalseType
{ };
// {
//     static constexpr bool value = false;
// };

template <typename T>
struct IsPointer<T*> : TrueType
{ };
// {
//     static constexpr bool value = true;
// };

template <typename T>
constexpr bool IsPointer_v = IsPointer<T>::value;

TEST_CASE("using traits")
{
    const int x = 10;
    auto buffer = need_for_trait(x);

    REQUIRE(buffer == std::array{10, 0, 0});

    // auto words = need_for_trait(std::string{"text"});

    static_assert(IsVoid_v<void>);
    static_assert(IsVoid_v<int> == false);

    static_assert(IsPointer_v<int*>);
    static_assert(IsPointer_v<int&> == false);
}