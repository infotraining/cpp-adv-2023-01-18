#include "utils.hpp"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

using Utils::Gadget;
using namespace std::literals;

template <typename T>
T maximum(T a, T b)
{
    return a < b ? b : a;
}

const char* maximum(const char* a, const char* b)
{
    return std::strcmp(a, b) < 0 ? b : a;
}

namespace Alt
{
    template <typename T1, typename T2>
    std::common_type_t<T1, T2> maximum(T1 a, T2 b)
    {
        return a < b ? b : a;
    }
} // namespace Alt

template <typename InpIter, typename OutIter, typename Predicate>
OutIter my_copy_if(InpIter first, InpIter last, OutIter dest, Predicate pred)
{
    for (InpIter it = first; it != last; ++it)
    {
        if (pred(*it))
        {
            *dest = *it;
            ++dest;
        }
    }

    return dest;
}

TEST_CASE("function templates")
{
    std::cout << maximum(4, 8) << "\n";

    std::cout << maximum("one"s, "two"s) << "\n";

    std::cout << maximum("one", "two") << "\n";

    auto result = Alt::maximum(3.14, 665);

    std::array data = {1, 4, 6, 3, 7, 665, 42, 6};
    std::vector<int> evens;

    my_copy_if(data.begin(), data.end(), std::back_inserter(evens), [](int n) { return n % 2 == 0; });

    REQUIRE(evens == std::vector{4, 6, 42, 6});
}

template <typename T1 = int, typename T2 = T1>
struct ValuePair
{
    T1 first{};
    T2 second{};

    ValuePair() = default;

    template <typename TFirst, typename TSecond>
    ValuePair(TFirst&& fst, TSecond&& snd)
        : first{std::forward<TFirst>(fst)}
        , second{std::forward<TSecond>(snd)}
    { }

    std::string to_string() const
    {
        std::stringstream ss;
        ss << "[ " << first << ", " << second << " ]";

        return ss.str();
    }
};

// deduction guide - since C++17
template <typename T1, typename T2>
ValuePair(T1, T2) -> ValuePair<T1, T2>;

TEST_CASE("class templates")
{
    ValuePair<int, double> v1{42, 3.14};
    std::cout << v1.to_string() << "\n";

    ValuePair<> v2{42, 665};
    std::cout << v2.to_string() << "\n";

    SECTION("Since C++17 - CTAD")
    {
        ValuePair vp1{42, 3.14}; // ValuePair<int, double>

        ValuePair vp2{42, 42}; // ValuePair<int, int>

        ValuePair vp3; // ValuePair<int, int>
        std::cout << vp3.to_string() << "\n";

        ValuePair vp4{"abc", 4};  // ValuePair<const char*, int>

        std::vector vec = {1, 2, 3, 4};
        std::array numbers = {1, 2, 3, 4, 5};
        std::optional opt = 42;
        std::unique_ptr<int> ptr{new int(13)};
        std::shared_ptr sptr = std::move(ptr);
    }
}

template <typename T, size_t N>
struct Array
{
    T items[N];

    using iterator = T*;
    using const_iterator = const T*;
};

template <typename TContainer>
void print_items(TContainer& container)
{
    for (typename TContainer::iterator it = container.begin(); it != container.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "\n";
}

TEST_CASE("tuple & dependent names")
{
    std::tuple<int, double> tpl1{1, 3.14};

    REQUIRE(std::get<0>(tpl1) == 1);
    // REQUIRE(tpl1.get<1>() == 3.14);
}

template <typename T>
using Buffer = std::array<T, 1024>;

TEST_CASE("template aliases")
{
    Buffer<uint8_t> buffer;
}

template <typename T>
constexpr T pi = 3.141592653589793238;

namespace Explain
{
    template <typename T>
    constexpr bool is_integral_v = std::is_integral<T>::value;
}

TEST_CASE("template variables")
{
    const double pi_d = 3.141592653589793238;
    const float pi_f = 3.141592653589793238;

    std::cout << pi<double> << "\n";
    std::cout << pi<float> << "\n";

    static_assert(std::is_integral<int>::value);
    static_assert(std::is_integral_v<int>); //  template variable
}