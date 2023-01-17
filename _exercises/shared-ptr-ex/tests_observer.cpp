#include <cassert>
#include <cstdlib>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <catch2/catch_test_macros.hpp>

class Observer
{
public:
    virtual void update(const std::string& event_args) = 0;
    virtual ~Observer() = default;
};

class Subject
{
    int state_;
    std::set<Observer*> observers_;

public:
    Subject() : state_(0)
    {
    }

    void register_observer(Observer* observer)
    {
        observers_.insert(observer);
    }

    void unregister_observer(Observer* observer)
    {
        observers_.erase(observer);
    }

    void set_state(int new_state)
    {
        if (state_ != new_state)
        {
            state_ = new_state;
            notify("Changed state on: " + std::to_string(state_));
        }
    }

protected:
    void notify(const std::string& event_args)
    {
        for (Observer* observer : observers_)
        {
            observer->update(event_args);
        }
    }
};

class ConcreteObserver1 : public Observer
{
public:
    virtual void update(const std::string& event)
    {
        std::cout << "ConcreteObserver1: " << event << std::endl;
    }
};

class ConcreteObserver2 : public Observer
{
public:
    virtual void update(const std::string& event)
    {
        std::cout << "ConcreteObserver2: " << event << std::endl;
    }
};

TEST_CASE("using observer pattern")
{
    // using namespace std;

    // Subject s;

    // ConcreteObserver1* o1 = new ConcreteObserver1;
    // s.register_observer(o1);

    // {
    //     ConcreteObserver2* o2 = new ConcreteObserver2;
    //     s.register_observer(o2);

    //     s.set_state(1);

    //     delete o2;

    //     cout << "End of scope." << endl;
    // }

    // s.set_state(2);
}
