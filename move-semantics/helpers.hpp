#include <iostream>
#include <string_view>

namespace Helpers
{
    template <typename TContainer>
    void print(const std::string& prefix, const TContainer& container)
    {
        std::cout << prefix << " - [ ";
        for (const auto& item : container)
        {
            std::cout << item << " ";
        }
        std::cout << "]\n";
    }
} // namespace Helpers