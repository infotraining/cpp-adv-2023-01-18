#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace std::literals;

std::string full_name(const std::string& first_name, const std::string& last_name)
{
    return first_name + " " + last_name;
}

TEST_CASE("reference binding")
{
    std::string name = "jan";

    SECTION("C++98")
    {
        // TODO
    }

    SECTION("C++11")
    {
        // TODO
    }
}