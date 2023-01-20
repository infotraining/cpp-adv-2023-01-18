#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <type_traits>

using namespace std;

void foo(int)
{ }

template <typename T>
void deduce1(T arg)
{
    puts(__PRETTY_FUNCTION__);
}

namespace Cpp20
{
    void deduce1(auto arg)
    {
        puts(__PRETTY_FUNCTION__);
    }

    void deduce2(auto& arg)
    {
        puts(__PRETTY_FUNCTION__);
    }

    void deduce3(auto&& arg)
    {
        puts(__PRETTY_FUNCTION__);
    }

} // namespace Cpp20

template <typename T>
void deduce2(T& arg)
{
    puts(__PRETTY_FUNCTION__);
}

template <typename T>
void deduce3(T&& arg)
{
    puts(__PRETTY_FUNCTION__);
}

TEST_CASE("Template Argument Deduction - case 1")
{
    int x = 10;
    deduce1(x);   // deduce1<int>(int)
    auto ax1 = x; // int

    const int cx = 10;
    deduce1(cx); // deduce1<int>(int)
    auto ax2 = cx; // int

    int& ref_x = x;
    deduce1(ref_x); // deduce1<int>(int)
    auto ax3 = ref_x; // int

    const int& cref_x = cx;
    deduce1(cref_x); // deduce1<int>(int)
    auto ax4 = cref_x; // int

    int tab[10];
    deduce1(tab); // deduce1<int*>(int*)
    auto ax5 = tab; // int*

    deduce1(foo); // deduce1<void(*)(int)>(void(*)(int))
    auto ax6 = foo; // void(*)(int)

    static_assert(std::is_same_v<std::decay_t<decltype(tab)>, int*>);
}

TEST_CASE("Template Argument Deduction - case 2")
{
    int x = 10;
    deduce2(x);  // deduce2<int>(int&)
    auto& ax1 = x; // int&

    const int cx = 10;
    deduce2(cx); // deduce2<const int>(const int&)
    auto& ax2 = cx; // const int&

    int& ref_x = x;
    deduce2(ref_x); // deduce2<int>(int&)
    auto& ax3 = ref_x; // int&

    const int& cref_x = cx;
    deduce2(cref_x); // deduce2<const int>(const int&)
    auto& ax4 = cref_x; // const int&

    int tab[10];
    deduce2(tab); // deduce2<int[10]>(int(&)[10])
    auto& ax5 = tab; // int(&)[10]

    deduce2(foo); // deduce2<void(int)>(void(&)(int))
    auto& ax6 = foo; // void(&)(int)
}

TEST_CASE("Template Argument Deduction - case 3")
{
    int x = 10;
    deduce3(x); // deduce3<int&>(int& && -> int&)
    auto&& ax1 = x; // int&

    const int cx = 10;
    deduce3(cx); // deduce3<const int&>(const int& && -> const int&)
    auto&& ax2 = cx; // const int&

    deduce3(x + x); // deduce3<int>(int&&)
    auto&& ax3 = x + x; // int&&
}

namespace Explain
{
    template <typename TContainer>
    constexpr size_t size(const TContainer& container) noexcept(noexcept(container.size()))
    {
        return container.size();
    }

    template <typename T, size_t N>
    constexpr size_t size(T (&container)[N]) noexcept
    {
        return N;
    }
}

TEST_CASE("size for containers")
{
    std::vector<int> vec = {1, 2, 3, 4};
    REQUIRE(Explain::size(vec) == 4);

    int tab[10];
    REQUIRE(std::size(tab) == 10);
}

////////////////////////////////////////////////////////////
// function return type deduction

using namespace std::literals;

auto bar(int x)
{
    if (x % 2 == 0)
        return "even"s; // std::string
    return "odd"s; // std::string
}

auto get_number()
{
    const int x = 42;
    return x;
}

template <typename TContainer>
decltype(auto) get_nth(TContainer& container, size_t n)
{
    return container.at(n);
}

TEST_CASE("function return type deduction")
{
    static_assert(std::is_same_v<decltype(bar(std::declval<int>())), std::string>);
    static_assert(std::is_same_v<decltype(get_number()), int>);

    std::vector<std::string> words = { "one", "two", "three" };
    get_nth(words, 1) = "dwa";
    REQUIRE(words[1] == "dwa");

    std::vector<bool> flags = {0, 1, 1, 0};
    get_nth(flags, 1) = false;
    REQUIRE(flags[1] == false);
}

struct X
{
private:
    X(int v, std::string s)
    {

    }
public:
    int sth()
    {
        return 42;
    }
};

auto foo(X x)
{
    return x.sth();
}

TEST_CASE("declval")
{
    using TReturn = decltype(foo(std::declval<X>())); 
}