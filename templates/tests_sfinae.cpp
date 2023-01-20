#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <cmath>

void foo(double d)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
    std::cout << "d: " << d << "\n";
}

// SFINAE in action
template <typename T>
typename T::type foo(T value)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
    std::cout << "value: " << value << "\n";
}

namespace Explain
{
    template <bool Condition, typename T>
    struct EnableIf
    {
        using type = T;
    };

    template <typename T>
    struct EnableIf<false, T>
    { };

    template <bool Condition, typename T>
    using EnableIf_t = typename EnableIf<Condition, T>::type;
} // namespace Explain

TEST_CASE("SFINAE")
{
    foo(3.14);
    foo(3.14f);
}

// SFINAE + EnableIf in action
template <typename T>
auto is_power_2(T value) -> typename Explain::EnableIf<std::is_integral_v<T>, bool>::type
{
    return value > 0 && (value & (value - 1)) == 0;
}

template <typename T>
auto is_power_2(T value) -> std::enable_if_t<std::is_floating_point_v<T>, bool>
{
    int exponent;
    const T mantissa = std::frexp(value, &exponent);
    return mantissa == static_cast<T>(0.5);
}

namespace Cpp17
{
    template <typename T>
    auto is_power_2(T value)
    {
        if constexpr(std::is_integral_v<T>)
        {
            return value > 0 && (value & (value - 1)) == 0;
        }
        else
        {
            int exponent;
            const T mantissa = std::frexp(value, &exponent);
            return mantissa == static_cast<T>(0.5);
        }        
    }
}

namespace Cpp20
{
    template <std::integral T>
    auto is_power_2(T value) 
    {
        return value > 0 && (value & (value - 1)) == 0;
    }

    template <std::floating_point T>
    auto is_power_2(T value)
    {
        int exponent;
        const T mantissa = std::frexp(value, &exponent);
        return mantissa == static_cast<T>(0.5);
    }
}

TEST_CASE("enable if")
{
    REQUIRE(is_power_2(8) == true);
    REQUIRE(is_power_2(64) == true);
    REQUIRE(is_power_2(31) == false);

    REQUIRE(is_power_2(8.0) == true);
}
