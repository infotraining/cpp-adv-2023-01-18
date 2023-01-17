#include "utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <vector>

using Utils::Gadget;

// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-resource

namespace LegacyCode
{
    // forward declarations
    Gadget* get_gadget(const std::string& name);
    void use(Gadget* g);
    void use_gadget(Gadget* g);

    // definitions
    Gadget* get_gadget(const std::string& name)
    {
        static int id = 665;
        return new Gadget(++id, name);
    }

    void use(Gadget* g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";

        delete g;
    }

    void use_gadget(Gadget* g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";
    }
} // namespace LegacyCode

TEST_CASE("legacy hell with dynamic memory")
{
    using namespace LegacyCode;

    {
        Gadget* g = get_gadget("ipad");

        use_gadget(g);
    } // memory leak

    {
        use_gadget(get_gadget("ipad"));
    } // memory leak

    {
        Gadget* g = new Gadget(13, "ipad");
        

        use(g);
        // use_gadget(g);  // UB!!! - use after delete
        // std::cout << g->name() << std::endl; // UB!!! - use after delete
    }

    {
        Gadget* g = get_gadget("ipad");
        

        use(g);

        // delete g; // UB!!! - second delete
    }
}
