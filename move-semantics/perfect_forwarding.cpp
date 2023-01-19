#include "gadget.hpp"

#include <catch2/catch_test_macros.hpp>
#include <deque>

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

using namespace Helpers;

////////////////////////////////////////////////////////
///  PERFECT FORWARDING

void have_fun(Gadget& g)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
    g.use();
}

void have_fun(const Gadget& cg)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
    cg.use();
}

void have_fun(Gadget&& g)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
    g.use();
}

namespace WithoutPerfectForwarding
{
    void use(Gadget& g)
    {
        have_fun(g);
    }

    void use(const Gadget& g)
    {
        have_fun(g);
    }

    void use(Gadget&& g)
    {
        have_fun(std::move(g));
    }

} // namespace WithoutPerfectForwarding

namespace WithPerfectForwarding
{
    namespace Cpp20
    {
        void use(auto&& g) // g is universal reference
        {
            have_fun(std::forward<decltype(g)>(g));
        }
    } // namespace Cpp20

    namespace Explain
    {
        template <typename T>
        decltype(auto) forward(T& item)
        {
            if constexpr (std::is_reference_v<T>)
            {
                std::cout << __PRETTY_FUNCTION__ << "\n";
                return item;
            }
            else
            {
                std::cout << __PRETTY_FUNCTION__ << "\n";
                return std::move(item);
            }
        }
    } // namespace Explain

    template <typename TGadget>
    void use(TGadget&& g) // g is universal reference
    {
        have_fun(std::forward<TGadget>(g));
    }
} // namespace WithPerfectForwarding

TEST_CASE("universal reference + auto")
{
    auto x = 10; // int

    auto&& ax1 = x; // int&

    auto&& ax2 = x + x; // int&&
}

TEST_CASE("using gadget - perfect forwarding")
{
    Gadget g{1, "g"};
    const Gadget cg{2, "const g"};

    using WithPerfectForwarding::use;

    use(g);
    use(cg);
    use(Gadget{3, "temporary gadget"});
}