#pragma once
#include <cassert>

template <typename T> class Singleton {
public:
    static T* Get()
    {
        if (!s_instance) {
            static T instance;
        }
        return s_instance;
    }

    Singleton()
    {
        assert(!s_instance && "Singleton already exists!");
        s_instance = static_cast<T*>(this);
    }

    virtual ~Singleton() { s_instance = nullptr; }

    // Prevent copying/moving
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

private:
    inline static T* s_instance = nullptr;
};

// template <typename T>
// T* Singleton<T>::s_instance = nullptr;
