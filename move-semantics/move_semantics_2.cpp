#include "gadget.hpp"

#include <catch2/catch_test_macros.hpp>

////////////////////////////////////////////////
// simplified implementation of unique_ptr - only moveable type

namespace Explain
{
    template <typename T>
    class unique_ptr
    {
    public:
        unique_ptr() noexcept
            : m_ptr{nullptr}
        {}

        unique_ptr(nullptr_t) noexcept : m_ptr{nullptr}
        {}

        explicit unique_ptr(T* value) noexcept : m_ptr{value}
        {
        }

        ~unique_ptr() noexcept
        {
            delete m_ptr;
        }

        unique_ptr(const unique_ptr&) = delete;
        unique_ptr& operator=(const unique_ptr&) = delete;

        // move constructor
        unique_ptr(unique_ptr&& rhs) noexcept : m_ptr{rhs.m_ptr}
        {
            rhs.m_ptr = nullptr;
        }

        // move assignment operator
        unique_ptr& operator=(unique_ptr&& rhs) noexcept
        {
            if (this != &rhs)
            {
                delete m_ptr;
                m_ptr = rhs.m_ptr;
                rhs.m_ptr = nullptr;
            }

            return *this;
        }

        T& operator*() const noexcept
        {
            return *m_ptr;
        }

        T* operator->() const noexcept
        {
            return m_ptr;
        }

        T* get() const noexcept
        {
            return m_ptr;
        }

    private:
        T* m_ptr;
    };

    template <typename T, typename... TArgs>
    unique_ptr<T> make_unique(TArgs&&... args)
    {
        return unique_ptr<T>{new T(std::forward<TArgs>(args)...)};
    }
} // namespace Explain

Explain::unique_ptr<Helpers::Gadget> create_gadget()
{
    static int id_gen = 0;
    const int id = ++id_gen;

    Explain::unique_ptr<Helpers::Gadget> ptr_g{new Helpers::Gadget{id, "Gadget#" + std::to_string(id)}};
    return ptr_g;
}


TEST_CASE("move semantics - unique_ptr")
{
    using Helpers::Gadget;

    Explain::unique_ptr<Gadget> ptr_gadget = Explain::make_unique<Gadget>(1, "ipad");
    ptr_gadget->use();
    (*ptr_gadget).use();

    Explain::unique_ptr<Gadget> ptr_target = std::move(ptr_gadget); // move constructor
    ptr_target->use();

    Explain::unique_ptr<Gadget> ptr_other = nullptr;

    ptr_other = std::move(ptr_target); // move assignment operator
    ptr_other->use();
}

TEST_CASE("move semantics - vector of unique_ptrs")
{
    using Helpers::Gadget;

    std::vector<Explain::unique_ptr<Gadget>> ptrs;

    ptrs.push_back(create_gadget());
    ptrs.push_back(create_gadget());

    Explain::unique_ptr<Gadget> ptr = create_gadget();
    ptrs.push_back(std::move(ptr));

    for(const auto& ptr_g : ptrs)
        ptr_g->use();
}