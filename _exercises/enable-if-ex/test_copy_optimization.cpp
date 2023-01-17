#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <list>
#include <string>
#include <vector>

using namespace std;

enum class Implementation {
    Generic,
    Optimized
};

template <typename InIter, typename OutIter>
Implementation mcopy(InIter start, InIter end, OutIter dest)
{
    for (auto it = start; it != end; ++it, ++dest)
    {
        *dest = *it;
    }

    return Implementation::Generic;
}

// TODO - optimized version with memcpy of mcopy

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

    // SECTION("optimized for arrays of POD types")
    // {
    //     int tab1[5] = {1, 2, 3, 4, 5};
    //     int tab2[5];

    //     REQUIRE(mcopy(begin(tab1), end(tab1), begin(tab2)) == Implementation::Optimized);
    //     REQUIRE(equal(begin(tab1), end(tab1), begin(tab2), end(tab2)));
    // }
}