#include <stdio.h>
#include <string>
#include <iostream>
#include <math.h>

class ReferenceCounter
{
public:
    explicit ReferenceCounter() : counter_(0U){};
    ReferenceCounter(const ReferenceCounter&) = delete;
    ReferenceCounter& operator=(const ReferenceCounter&) = delete;

    /// Increment counter by one.
    void increment() { ++counter_; };

    /// Decrement counter by one.
    void decrement() { --counter_; };

    /// Returns value of counter.
    uint32_t get_count() const { return counter_; };

private:
    uint32_t counter_;
};

template<typename T>
class SharedPtr
{
public:
    explicit SharedPtr(T* other = nullptr);
    explicit SharedPtr(const SharedPtr<T>& other);
    int32_t get_count() const;
    ~SharedPtr();

private:
    T*                raw_ptr;
    ReferenceCounter* counter;
};

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other)
{
    raw_ptr = other.raw_ptr;
    counter = other.counter;
    
    if (counter == nullptr)
    {
        counter = new ReferenceCounter();
    }

    counter->increment();
    std::cout << "Counter for Copy constructor: " << counter->get_count() << std::endl;
}

template<typename T>
int32_t SharedPtr<T>::get_count() const
{
    if (counter)
    {
       return counter->get_count();
    }

    return 0;
}

template<typename T>
SharedPtr<T>::SharedPtr(T* other)
{
    raw_ptr = other;
    counter = new ReferenceCounter();
    counter->increment();
    std::cout << "Counter for constructor: " << counter->get_count() << std::endl;
}

template<typename T>
SharedPtr<T>::~SharedPtr()
{
    counter->decrement();
    std::cout << "Counter for destructor: " << counter->get_count() << std::endl;
    if (counter->get_count() <= 0)
    {
        if (raw_ptr)
        {
            delete raw_ptr;
            raw_ptr = nullptr;
        }
        delete counter;
    }
}
