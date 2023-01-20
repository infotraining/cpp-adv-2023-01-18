#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <optional>

namespace Cpp11
{
    constexpr uint64_t factorial(uint64_t n)
    {
        return (n == 0) ? 1 : n * factorial(n - 1);
    }
} // namespace Cpp11

constexpr uint64_t factorial(uint64_t n)
{
    if (n == 0)
    {
        return 1;
    }
    else 
    {
        return n * factorial(n - 1);
    }
}

template <size_t N>
constexpr auto create_factorial_lookup_table()
{
    std::array<uint64_t, N> values{};

    for(uint64_t i = 0; i < N; ++i)
        values[i] = factorial(i);

    return values;
}

namespace Cpp17
{
    template <typename Iter, typename Predicate>
    constexpr auto find_if(Iter first, Iter last, Predicate pred)
    {
        using T = std::iterator_traits<Iter>::value_type;

        for(auto it = first; it != last; ++it)
        {
            if (pred(*it))
                return std::optional<T>{*it};
        }

        return std::optional<T>{};
    }
}

TEST_CASE("constexpr")
{
    constexpr size_t buffer_size = 1024;
    constexpr size_t large_buffer_size = buffer_size * 10;

    constexpr auto fact_8 = factorial(8);

    int n = 4;
    std::cout << factorial(n) << "\n";

    constexpr auto lookup_table = create_factorial_lookup_table<10>();

    static_assert(std::size(lookup_table) == 10);
    static_assert(lookup_table[0] == 1);

    static_assert(Cpp17::find_if(lookup_table.begin(), lookup_table.end(), [](auto n) { return n > 1000; }).value() == 5040);
}