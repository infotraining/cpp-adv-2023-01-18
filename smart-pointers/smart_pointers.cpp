#include "utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <functional>
#include <memory>
#include <vector>
#include <map>

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

    int* create_buffer(unsigned int size)
    {
        int* buffer = new int[size];

        std::fill_n(buffer, size, 0);

        return buffer;
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

TEST_CASE("unique_ptr - special abilities")
{
    SECTION("controlling dynamic arrays")
    {
        std::unique_ptr<int[]> my_array{LegacyCode::create_buffer(1024)};

        my_array[6] = 665;
    } // my_array will be destroyed - delete[] is called

    SECTION("custom deallocators")
    {
        SECTION("Legacy style")
        {
            FILE* file_txt = fopen("file.txt", "w+");

            if (!file_txt)
                std::terminate();

            fputs("text", file_txt);

            //... may throw

            fclose(file_txt);
        }

        SECTION("RAII style")
        {
            {
                std::unique_ptr<FILE, std::function<int(FILE*)>> file_txt{fopen("file.txt", "w+"), [](FILE* f) {
                                                                              std::cout << "Closing file: " << f << "\n";
                                                                              return fclose(f);
                                                                          }};

                if (!file_txt)
                    std::terminate();

                fputs("text", file_txt.get());
            }

            {
                std::string filename = "FILE.TXT";

                auto file_closer = [filename](FILE* f) {
                    std::cout << "Closing file: " << filename << "\n";
                    return fclose(f);
                };

                std::unique_ptr<FILE, decltype(file_closer)> file_TXT{fopen(filename.c_str(), "w+"), file_closer};

                if (!file_TXT)
                    std::terminate();

                fputs("text", file_TXT.get());
            }
        }
    }
    //... may throw
} // destructor of file_txt - fclose() is called


TEST_CASE("shared pointers")
{
    std::map<std::string, std::shared_ptr<Gadget>> dict_gadgets;
    std::weak_ptr<Gadget> wptr_gadget;

    {
        //std::shared_ptr<Gadget> sp1{new Gadget{1, "ipad"}};
        auto sp1 = std::make_shared<Gadget>(1, "ipad");
        REQUIRE(sp1.use_count() == 1);

        std::shared_ptr<Gadget> sp2 = sp1;
        REQUIRE(sp1.use_count() == 2);

        dict_gadgets.emplace("ipad", sp1);
        REQUIRE(sp1.use_count() == 3);

        wptr_gadget = sp1;
        REQUIRE(sp1.use_count() == 3);
    }
    REQUIRE(dict_gadgets["ipad"].use_count() == 1);

    std::cout << *dict_gadgets["ipad"] << "\n";

    if (std::shared_ptr<Gadget> sptr = wptr_gadget.lock())
    {
        std::cout << "Still alive: " << *sptr << "\n";
    } 

    dict_gadgets.clear(); // ref count is zero -> gadget is destroyed

    if (std::shared_ptr<Gadget> sptr = wptr_gadget.lock())
    {
        std::cout << "Still alive: " << *sptr << "\n";
    } 
    else
    {
        std::cout << "Gadget has been destroyed\n";
    }

    SECTION("Custom deallocators")
    {
        SECTION("RAII style")
        {
            {
                std::shared_ptr<FILE> file_txt{fopen("file.txt", "w+"), [](FILE* f) {
                                                                              std::cout << "Closing file: " << f << "\n";
                                                                              if (f)
                                                                                fclose(f);
                                                                        }};

                if (file_txt)
                    fputs("text", file_txt.get());
            }
        }
    }
}