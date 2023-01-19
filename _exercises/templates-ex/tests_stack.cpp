#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <deque>
#include <memory>

template <typename T, template <typename, typename> class Container = std::deque, typename TAllocator = std::allocator<T>>
class Stack
{
    Container<T, TAllocator> m_buffer;

public:
    bool empty() const { return m_buffer.empty(); }

    std::size_t size() const { return m_buffer.size(); }

    // void push(const T& item)
    // {
    //     m_buffer.push_back(item);
    // }

    // void push(T&& item)
    // {
    //     m_buffer.push_back(std::move(item));
    // }

    template <typename TItem>
    void push(TItem&& item)
    {
        m_buffer.push_back(std::forward<TItem>(item));
    }

    template <typename... TArgs>
    void emplace(TArgs&&... args)
    {
        m_buffer.emplace_back(std::forward<TArgs>(args)...);
    }

    T& top()
    {
        return m_buffer.back();
    }

    void pop()
    {
        m_buffer.pop_back();
    }
};

TEST_CASE("After construction", "[stack,constructors]")
{
    Stack<int, std::vector> s;

    SECTION("is empty")
    {
        REQUIRE(s.empty());
    }

    SECTION("size is zero")
    {
        REQUIRE(s.size() == 0);
    }
}

SCENARIO("Stack - push")
{
    GIVEN("An empty stack")
    {
        Stack<int> s; // Arrange

        WHEN("item pushed to stack")
        {
            auto size_before = s.size();
            s.push(1); // Act

            THEN("is no longer empty")
            {
                REQUIRE(!s.empty()); // Assert
            }

            THEN("size is increased")
            {
                REQUIRE(s.size() - size_before == 1);
            }

            THEN("recently pushed item is on a top")
            {
                REQUIRE(s.top() == 1);
            }
        }
    }
}

TEST_CASE("emplace")
{
    Stack<std::string> s;

    s.emplace(5, 'a');

    REQUIRE(s.top() == "aaaaa");
}

template <typename T>
std::vector<T> pop_all(Stack<T>& s)
{
    std::vector<T> values(s.size());

    for (auto& item : values)
    {
        item = std::move(s.top());
        s.pop();
    }

    return values;
}

TEST_CASE("Popping an item", "[stack,pop]")
{
    Stack<int> s;

    s.push(1);
    s.push(4);

    int item;

    SECTION("assigne an item from a top to an argument passed by ref")
    {
        item = s.top();
        s.pop();

        REQUIRE(item == 4);
    }

    SECTION("size is decreased")
    {
        size_t size_before = s.size();

        item = s.top();
        s.pop();

        REQUIRE(size_before - s.size() == 1);
    }

    SECTION("LIFO order")
    {
        int a, b;

        a = s.top();
        s.pop();

        b = s.top();
        s.pop();

        REQUIRE(a == 4);
        REQUIRE(b == 1);
    }
}

TEST_CASE("Move semantics", "[stack,push,pop,move]")
{
    using namespace std::literals;

    SECTION("stores move-only objects")
    {
        auto txt1 = std::make_unique<std::string>("test1");

        Stack<std::unique_ptr<std::string>> s;

        s.push(move(txt1));
        s.push(std::make_unique<std::string>("test2"));

        std::unique_ptr<std::string> value;

        value = std::move(s.top());
        s.pop();
        REQUIRE(*value == "test2"s);

        value = std::move(s.top());
        s.pop();
        REQUIRE(*value == "test1"s);
    }

    SECTION("move constructor", "[stack,move]")
    {
        Stack<std::unique_ptr<std::string>> s;

        s.push(std::make_unique<std::string>("txt1"));
        s.push(std::make_unique<std::string>("txt2"));
        s.push(std::make_unique<std::string>("txt3"));

        auto moved_s = std::move(s);

        auto values = pop_all(moved_s);

        auto expected = {"txt3", "txt2", "txt1"};
        REQUIRE(std::equal(values.begin(), values.end(), expected.begin(), [](const auto& a, const auto& b) { return *a == b; }));
    }

    SECTION("move assignment", "[stack,move]")
    {
        Stack<std::unique_ptr<std::string>> s;

        s.push(std::make_unique<std::string>("txt1"));
        s.push(std::make_unique<std::string>("txt2"));
        s.push(std::make_unique<std::string>("txt3"));

        Stack<std::unique_ptr<std::string>> target;
        target.push(std::make_unique<std::string>("x"));

        target = std::move(s);

        REQUIRE(target.size() == 3);

        auto values = pop_all(target);

        auto expected = {"txt3", "txt2", "txt1"};
        REQUIRE(std::equal(values.begin(), values.end(), expected.begin(), [](const auto& a, const auto& b) { return *a == b; }));
    }
}
