#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <iostream>
#include <list>
#include <string>
#include <vector>

using namespace std;

enum class Implementation {
    Generic,
    Optimized
};

namespace SFINAE
{
    template <typename InIter, typename OutIter>
    Implementation mcopy(InIter start, InIter end, OutIter dest)
    {
        for (auto it = start; it != end; ++it, ++dest)
        {
            *dest = *it;
        }

        return Implementation::Generic;
    }

    template <typename T>
    auto mcopy(T* start, T* end, T* dest) -> std::enable_if_t<std::is_trivially_copyable_v<T>, Implementation>
    {
        memcpy(dest, start, (end - start) * sizeof(T));
        return Implementation::Optimized;
    }
} // namespace SFINAE

template <typename InIter, typename OutIter>
Implementation mcopy(InIter start, InIter end, OutIter dest)
{
    using TIn = typename std::iterator_traits<InIter>::value_type;
    using TOut = typename std::iterator_traits<OutIter>::value_type;
    std::cout << "TIn: " << typeid(TIn).name() << "\n";
    std::cout << "TOut: " << typeid(TOut).name() << "\n";

    if constexpr (std::is_same_v<TIn, TOut>
        && std::is_trivially_copyable_v<TIn>
        && std::is_pointer_v<InIter> && std::is_pointer_v<OutIter>)
    {
        memcpy(dest, start, (end - start) * sizeof(TIn));
        return Implementation::Optimized;
    }
    else
    {
        for (auto it = start; it != end; ++it, ++dest)
        {
            *dest = *it;
        }

        return Implementation::Generic;
    }
}

TEST_CASE("mcopy")
{
    SECTION("generic version for STL containers")
    {
        vector<int> vec = {1, 2, 3, 4, 5};
        list<int> lst(5);

        REQUIRE(mcopy(vec.begin(), vec.end(), lst.begin()) == Implementation::Generic);
        REQUIRE(equal(vec.begin(), vec.end(), lst.begin(), lst.end()));
    }

    SECTION("generic for array of strings")
    {
        const string words[] = {"1", "2", "3"};
        string dest[3];

        REQUIRE(mcopy(begin(words), end(words), begin(dest)) == Implementation::Generic);
        REQUIRE(equal(begin(words), end(words), begin(dest), end(dest)));
    }

    SECTION("optimized for arrays of POD types")
    {
        int tab1[5] = {1, 2, 3, 4, 5};
        int tab2[5];

        static_assert(std::is_same_v<std::iterator_traits<decltype(std::begin(tab1))>::iterator_concept, std::contiguous_iterator_tag>);

        REQUIRE(mcopy(begin(tab1), end(tab1), begin(tab2)) == Implementation::Optimized);
        REQUIRE(equal(begin(tab1), end(tab1), begin(tab2), end(tab2)));
    }
}

template <typename T>
void push_back(T&& obj)
{
    
    // if constexpr (std::is_nothrow_move_constructible<std::decay_t<T>>)
    // {   
    //     T value = std::move(obj);    
    // }
    // else
    // {
    //     T value = obj;
    // }

    T value = std::move_if_noexcept(obj);
}