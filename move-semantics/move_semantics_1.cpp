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
        std::string& ref_name = name;  

        const std::string& ref_full_name = full_name("jan", "kowalski");

        //ref_full_name[0] = 'p'; // ERROR
    }

    SECTION("C++11")
    {
        std::string&& rv_ref_full_name = full_name("jan", "kowalski");

        rv_ref_full_name[0] = 'p';  // OK

        // std::string&& rv_ref_name = name; // ERROR
    }
}

namespace Explain
{
    class vector
    {
    public:
        void push_back(const std::string& item)
        {
            std::cout << item << " is copied to a vector\n";
        }

        void push_back(std::string&& item)
        {
            std::cout << item << " is moved to a vector\n";
        }
    };
}

TEST_CASE("using push_back")
{
    std::string name = "jan";

    Explain::vector vec;

    vec.push_back(name);
    vec.push_back("adam");
    vec.push_back(name + name);
    vec.push_back(std::move(name));

    name = "ewa"; // OK - assignment after move is OK
}