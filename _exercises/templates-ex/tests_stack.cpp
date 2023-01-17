#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <array>
#include <memory>

// TEST_CASE("After construction", "[stack,constructors]")
// {
//     Stack<int> s;

//     SECTION("is empty")
//     {
//         REQUIRE(s.empty());
//     }

//     SECTION("size is zero")
//     {
//         REQUIRE(s.size() == 0);
//     }
// }

// TEST_CASE("Pushing an item", "[stack,push]")
// {
//     Stack<int> s;

//     SECTION("is no longer empty")
//     {
//         s.push(1);

//         REQUIRE(!s.empty());
//     }

//     SECTION("size is increased")
//     {
//         auto size_before = s.size();

//         s.push(1);

//         REQUIRE(s.size() - size_before == 1);
//     }

//     SECTION("recently pushed item is on a top")
//     {
//         s.push(4);

//         REQUIRE(s.top() == 4);
//     }
// }

// template <typename T>
// std::vector<T> pop_all(Stack<T>& s)
// {
//     std::vector<T> values(s.size());

//     for (auto& item : values)
//     {
//         item = std::move(s.top());
//         s.pop();
//     }

//     return values;
// }

// TEST_CASE("Popping an item", "[stack,pop]")
// {
//     Stack<int> s;

//     s.push(1);
//     s.push(4);

//     int item;

//     SECTION("assignes an item from a top to an argument passed by ref")
//     {
//         item = s.top();
//         s.pop();

//         REQUIRE(item == 4);
//     }

//     SECTION("size is decreased")
//     {
//         size_t size_before = s.size();

//         item = s.top();
//         s.pop();


//         REQUIRE(size_before - s.size() == 1);
//     }

//     SECTION("LIFO order")
//     {
//         int a, b;

//         a = s.top();
//         s.pop();

//         b = s.top();
//         s.pop();


//         REQUIRE(a == 4);
//         REQUIRE(b == 1);
//     }
// }

// TEST_CASE("Move semantics", "[stack,push,pop,move]")
// {
//     using namespace std::literals;

//     SECTION("stores move-only objects")
//     {
//         auto txt1 = std::make_unique<std::string>("test1");

//         Stack<std::unique_ptr<std::string>> s;

//         s.push(move(txt1));
//         s.push(std::make_unique<std::string>("test2"));

//         std::unique_ptr<std::string> value;

//         value = std::move(s.top());
//         s.pop();
//         REQUIRE(*value == "test2"s);

//         value = std::move(s.top());
//         s.pop();
//         REQUIRE(*value == "test1"s);
//     }

//     SECTION("move constructor", "[stack,move]")
//     {
//         Stack<std::unique_ptr<std::string>> s;

//         s.push(std::make_unique<std::string>("txt1"));
//         s.push(std::make_unique<std::string>("txt2"));
//         s.push(std::make_unique<std::string>("txt3"));

//         auto moved_s = std::move(s);

//         auto values = pop_all(moved_s);

//         auto expected = {"txt3", "txt2", "txt1"};
//         REQUIRE(std::equal(values.begin(), values.end(), expected.begin(), [](const auto& a, const auto& b) { return *a == b; }));
//     }

//     SECTION("move assignment", "[stack,move]")
//     {
//         Stack<std::unique_ptr<std::string>> s;

//         s.push(std::make_unique<std::string>("txt1"));
//         s.push(std::make_unique<std::string>("txt2"));
//         s.push(std::make_unique<std::string>("txt3"));

//         Stack<std::unique_ptr<std::string>> target;
//         target.push(std::make_unique<std::string>("x"));

//         target = std::move(s);

//         REQUIRE(target.size() == 3);

//         auto values = pop_all(target);

//         auto expected = {"txt3", "txt2", "txt1"};
//         REQUIRE(std::equal(values.begin(), values.end(), expected.begin(), [](const auto& a, const auto& b) { return *a == b; }));
//     }
// }