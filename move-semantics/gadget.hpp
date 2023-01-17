#ifndef GADGET_HPP
#define GADGET_HPP

#include <iostream>
#include <string>

namespace Helpers
{
    struct Gadget
    {
        int id{};
        std::string name{"not-set"};

        Gadget() = default;

        explicit Gadget(int v)
            : id{v}
        {
            std::cout << "Gadget(" << id << ")\n";
        }

        Gadget(int v, const std::string& n)
            : id{v}
            , name{n}
        {
            std::cout << "Gadget(" << id << ", " << name << ")\n";
        }

        Gadget(const Gadget&) = default;
        Gadget& operator=(const Gadget&) = default;
        Gadget(Gadget&&) = default;
        Gadget& operator=(Gadget&&) = default;

        ~Gadget()
        {
            std::cout << "~Gadget(" << id << ", " << name << ")\n";
        }

        void use() const
        {
            std::cout << "Using Gadget(" << id << ", " << name << ")\n";
        }
    };
} // namespace Helpers

#endif