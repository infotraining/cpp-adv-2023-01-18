#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <memory>
#include <algorithm>
#include <tuple>
#include <numeric>
#include <variant>

namespace VariadicTemplates
{
    template <typename... Ts>
    struct TypeList
    {
        constexpr static size_t value = sizeof...(Ts);
    };

    template <typename T, typename... TArgs>
    std::unique_ptr<T> make_unique(TArgs&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<TArgs>(args)...));
    }

    // template <typename T, int, char>
    // std::unique_ptr<T> make_unique(int&& arg1, char&& arg2)
    // {
    //     return std::unique_ptr<T>(new T(std::forward<int>(arg1), std::forward<char>(arg2)));
    // }

} // namespace VariadicTemplates

template <typename... Ts>
struct Row
{
    std::tuple<Ts...> data;
};

TEST_CASE("variadic templates")
{
    using VariadicTemplates::TypeList;

    TypeList<int, double, std::string, int> tl1;

    static_assert(decltype(tl1)::value == 4);
    static_assert(TypeList<>::value == 0);

    auto ptr = VariadicTemplates::make_unique<std::string>(10, 'a');
}

struct Anonymous
{
    int val1;
    double val2;
};

TEST_CASE("tuple described")
{
    std::tuple<int, double> tpl1{42, 3.24};

    REQUIRE(std::get<0>(tpl1) == 42);
}

//////////////////////////////////////
// Head-Tail idiom

void print()
{
    std::cout << "\n";
}

template <typename Head, typename... Tail>
void print(Head head, Tail... tail)
{
    std::cout << head << " ";
    print(tail...);
}

namespace Cpp17
{
    template <typename Head, typename... Tail>
    void print(Head head, Tail... tail)
    {
        std::cout << head << " ";

        if constexpr (sizeof...(tail) > 0)
            print(tail...);
        else
            std::cout << "\n";
    }
} // namespace Cpp17

TEST_CASE("head-tail")
{
    Cpp17::print(1, 3.14, "test", std::string("str"));
}

//////////////////////////////////////////////////////////
// Fold expressions

namespace BeforeCpp17
{
    template <typename T>
    auto sum(T value)
    {
        return value;
    }

    template <typename Head, typename... Tail>
    auto sum(Head head, Tail... tail)
    {
        return head + sum(tail...);
    }
} // namespace BeforeCpp17

template <typename... TArgs>
auto sum(TArgs... args)
{
    return (... + args);  // left fold expression
    //return ((((1 + 2) + 3) + 4) + 5);
}

template <typename... TArgs>
auto sum_r(TArgs... args)
{
    return (args + ...);  // right fold expressions
}

template <typename... TArgs>
void fold_print(const TArgs&... args)
{
    bool is_first = true;

    auto with_space = [&is_first](const auto& arg) {
        if (!is_first)
            std::cout << " ";
        else    
            is_first = false;
        return arg;
    };

    (std::cout << ... << with_space(args)) << "\n";
}

template <typename... TArgs>
void print_lines(const TArgs&... args)
{
    (..., (std::cout << args << "\n"));
}

TEST_CASE("fold expressions")
{
    using namespace std::literals;

    REQUIRE(sum(1, 2, 3, 4, 5) == 15);
    REQUIRE(sum("1-"s, "2-"s, "3") == "1-2-3");
    fold_print(1, 3.14, "text"s);
    print_lines(1, 3.14, "text"s);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

class Gadget
{
public:
    virtual std::string id() const { return "a"; }
    virtual ~Gadget() = default;
};

class SuperGadget : public Gadget
{
public:
    std::string id() const override
    {
        return "b";
    }
};


template <typename... Args>
auto make_vector(Args&&... args)
{
    std::vector<std::common_type_t<Args...>> vect;

    vect.reserve(sizeof...(Args));
    
    (..., vect.emplace_back(std::forward<Args>(args))); 
    
    return vect;
}

template <typename F, typename... TArgs>
void apply_f(F&& f, TArgs&&... args)
{
    (..., f(args));
}

TEST_CASE("apply_f")
{
    apply_f([](const auto& v) { std::cout << v << "\n"; }, 1, 3.14, "text");
}

TEST_CASE("make_vector - create vector from a list of arguments")
{
    // Tip: use std::common_type_t<Ts...> trait

    SECTION("ints")
    {
        std::vector<int> v = make_vector(1, 2, 3);

        REQUIRE(v == std::vector{1, 2, 3});
    }

    SECTION("unique_ptrs")
    {
        auto ptrs = make_vector(std::make_unique<int>(5), std::make_unique<int>(6));

        REQUIRE(ptrs.size() == 2);
    }

    SECTION("unique_ptrs with polymorphic hierarchy")
    {
        using namespace std;

        auto gadgets = make_vector(make_unique<Gadget>(), make_unique<SuperGadget>(), make_unique<Gadget>());

        static_assert(is_same_v<decltype(gadgets)::value_type, unique_ptr<Gadget>>);

        vector<string> ids;
        transform(begin(gadgets), end(gadgets), back_inserter(ids), [](auto& ptr) { return ptr->id(); });

        REQUIRE(ids  == vector<string>{"a", "b", "a"});        
    }
}

////////////////////////////////////////////////////////////
// tuples

std::tuple<int, int, double> calc_stats(const std::vector<int>& data)
{
    auto [pos_min, pos_max] = std::minmax_element(data.begin(), data.end());
    double avg = std::accumulate(data.begin(), data.end(), 0.0) / data.size();

    return std::tuple{*pos_min, *pos_max, avg};
}

TEST_CASE("using tuples")
{
    std::vector data = {7, 53, 665, 3, 8};

    auto [min, max, avg] = calc_stats(data);

    REQUIRE(min == 3);
    REQUIRE(max == 665);
}

struct Data
{
    int value;
};

[[nodiscard]] std::variant<Data, std::errc> parse_data(const std::string& str)
{
    try
    {
        Data data{std::stoi(str)};
        return data;
    }
    catch(...)
    {
        return std::errc::invalid_argument;
    }
}

struct F
{
    void operator()(int x) { std::cout << "x: " << x << "\n"; }
};

struct G
{
    
};


template <typename... Ts>
struct overload : Ts...
{
    using Ts::operator()...;
};

using FG = overload<F, G>;

// deduction guide
template <typename... Ts>
overload(Ts...) -> overload<Ts...>;

template <typename TVariant, typename... THandlers>
decltype(auto) process_result(TVariant&& result, THandlers&&... handlers)
{
    auto visitor = overload{
        std::forward<THandlers>(handlers)...
    };

    return std::visit(visitor, result);
} 

TEST_CASE("using variants in return type")
{
    process_result(parse_data("x"), 
        [](const Data& d) { std::cout << "Parsed data: " << d.value << "\n"; },
        [](std::errc ec) { std::cout << "Error#" << static_cast<int>(ec) << "\n";}
    );
}

