#pragma once

#include <vector>
#include <functional>
#include <algorithm>

template<typename... Args>
class Delegate
{
public:

    struct Handle
    {
        size_t id = 0;
        void* owner = nullptr;

        bool IsValid() const
        {
            return id != 0;
        }
    };

private:

    struct Listener
    {
        Handle handle;
        std::function<void(Args...)> func;
    };

public:

    Delegate() : nextId(1) {}

    Handle Add(std::function<void(Args...)> func)
    {
        Handle h;
        h.id = nextId++;
        h.owner = nullptr;

        listeners.push_back({ h, func });

        return h;
    }

    template<typename T>
    Handle Add(T* instance, void(T::* method)(Args...))
    {
        Handle h;
        h.id = nextId++;
        h.owner = instance;

        listeners.push_back({
            h,
            [=](Args... args)
            {
                (instance->*method)(args...);
            }
            });

        return h;
    }

    void Remove(Handle handle)
    {
        listeners.erase(
            std::remove_if(listeners.begin(), listeners.end(),
                [&](const Listener& l)
                {
                    return l.handle.id == handle.id &&
                        l.handle.owner == handle.owner;
                }),
            listeners.end());
    }

    void RemoveAll(void* owner)
    {
        listeners.erase(
            std::remove_if(listeners.begin(), listeners.end(),
                [&](const Listener& l)
                {
                    return l.handle.owner == owner;
                }),
            listeners.end());
    }
    void RemoveAll() {
        listeners.erase(listeners.begin(), listeners.end());
    }
    void Broadcast(Args... args)
    {
        for (auto& l : listeners)
        {
            l.func(args...);
        }
    }

private:
    std::vector<Listener> listeners;
    size_t nextId;
};

#define DECLARE_DELEGATE(Name, ...) \
    using Name = Delegate<__VA_ARGS__>