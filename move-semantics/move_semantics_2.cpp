#include "gadget.hpp"

#include <catch2/catch_test_macros.hpp>

////////////////////////////////////////////////
// simplified implementation of unique_ptr - only moveable type

namespace Explain
{
    template <typename T>
    class unique_ptr
    {
        // TODO
    };
} // namespace Explain

TEST_CASE("move semantics - unique_ptr")
{
}