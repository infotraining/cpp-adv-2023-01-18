#include "helpers.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>

////////////////////////////////////////////////////////////////////////////
// Data - class with copy & move semantics (user provided implementation)

using namespace Helpers;

class Data
{
    std::string name_;
    int* data_;
    size_t size_;

public:
    using iterator = int*;
    using const_iterator = const int*;

    Data(std::string name, std::initializer_list<int> list)
        : name_{std::move(name)}
        , size_{list.size()}
    {
        data_ = new int[list.size()];
        std::copy(list.begin(), list.end(), data_);

        std::cout << "Data(" << name_ << ")\n";
    }

    Data(const Data& other)
        : name_(other.name_)
        , size_(other.size_)
    {
        std::cout << "Data(" << name_ << ": cc)\n";
        data_ = new int[size_];
        std::copy(other.begin(), other.end(), data_);
    }

    Data& operator=(const Data& other)
    {
        Data temp(other);
        swap(temp);

        std::cout << "Data=(" << name_ << ": cc)\n";

        return *this;
    }

    // TODO: move semantics
    Data(Data&& source) noexcept : name_(std::move(source.name_)), data_(source.data_), size_(source.size_)
    {
        source.data_ = nullptr;
        source.size_ = 0;

        std::cout << "Data(" << name_ << ": mv)\n";
    }

    Data& operator=(Data&& source)
    {
        // if(this != &source)
        // {
        //     delete[] data_;

        //     name_ = std::move(source.name_);
            
        //     data_ = source.data_;
        //     size_ = source.size_;
        //     source.data_ = nullptr;
        //     source.size_ = 0;
        // }

        if (this != &source)
        {
            Data temp = std::move(source);
            swap(temp);
        }
        std::cout << "Data=(" << name_ << ": mv)\n";

        return *this;
    }

    ~Data() noexcept
    {
        delete[] data_;
    }

    void swap(Data& other) 
    {
        name_.swap(other.name_);
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    iterator begin() noexcept
    {
        return data_;
    }

    iterator end() noexcept
    {
        return data_ + size_;
    }

    const_iterator begin() const noexcept
    {
        return data_;
    }

    const_iterator end() const noexcept
    {
        return data_ + size_;
    }
};

Data create_data_set()
{
    Data ds{"data-set-one", {54, 6, 34, 235, 64356, 235, 23}};

    return ds;
}

TEST_CASE("Data & move semantics")
{
    Data ds1{"ds1", {1, 2, 3, 4, 5}};

    Data backup = ds1; // copy
    print("backup", backup);

    Data ds2 = create_data_set();
    print("ds2", ds2);

    Data ds3 = std::move(backup);
    print("ds3", ds3);

    ds3 = create_data_set();
    print("ds3", ds3);
}

class DataSet
{
    std::string name_;
    Data row1_;
    Data row2_;
public:
    DataSet(std::string name, Data row1, Data row2)
        : name_(std::move(name))
        , row1_(std::move(row1))
        , row2_(std::move(row2))
    {
    }

    // template<typename T1, typename T2, typename T3>
    // DataSet(T1&& name, T2&& row1, T3&& row2) : name_(std::forward<T1>(name))
    //     , row1_(std::forward<T2>(row1))
    //     , row2_(std::forward<T3>(row2))
    // {        
    // }

    DataSet(const DataSet&) = default;
    DataSet& operator=(const DataSet&) = default;
    DataSet(DataSet&&) = default;
    DataSet& operator=(DataSet&&) = default;

    ~DataSet() 
    {
        std::cout << "Log: " << name_ << "\n";
    }

    void print_rows() const
    {
        std::cout << name_ << "\n";
        print("1", row1_);
        print("2", row2_);
    }
};

TEST_CASE("DataSet")
{
    std::string name = "ds1";
    DataSet ds1{name, Data{"a", {1, 2, 3}}, Data{"b", {3, 5, 7}}};

    ds1.print_rows();

    DataSet backup = ds1; // cc

    DataSet target = std::move(backup); // mv

    target.print_rows();
}

struct AllByDefault
{
    std::string id;
    std::vector<int> vec;    

    AllByDefault() = default;

    AllByDefault(std::string id) : id{std::move(id)}
    {}

    ~AllByDefault() {}
};

TEST_CASE("Rules for special functions")
{
    AllByDefault a1;
}

// template <typename T>
// struct Wrapper
// {
//     T object;

//     template <typename TArg>
//     Wrapper(TArg&& value) : object(std::forward<TArg>(value))
//     {}
// };

TEST_CASE("noexcept")
{
    std::vector<Data> vec;

    vec.push_back(create_data_set());
    vec.push_back(Data{"ds2", {1, 2, 3}});
    std::cout << "---------\n";
    vec.push_back(Data{"ds3", {1, 2, 3}});
    std::cout << "---------\n";
    vec.push_back(Data{"ds4", {1, 2, 3}});
    std::cout << "---------\n";
    vec.push_back(Data{"ds5", {1, 2, 3}});
}