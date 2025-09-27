#pragma once
#include <algorithm>
#include <functional>
#include <vector>

// Simple Signal/Slot with disconnect functionality
template <typename... Args> class Signal {
private:
    struct Slot {
        std::function<void(Args...)> func;
        bool connected = true;
        size_t id;
    };

    mutable std::vector<Slot> slots;
    size_t nextId = 0;

public:
    // Connect and return an ID for disconnecting
    size_t connect(std::function<void(Args...)> f)
    {
        size_t id = nextId++;
        slots.push_back({ std::move(f), true, id });
        return id;
    }

    // Disconnect by ID
    void disconnect(size_t id)
    {
        auto it = std::find_if(slots.begin(), slots.end(), [id](const Slot& slot) { return slot.id == id; });

        if (it != slots.end()) {
            it->connected = false;
        }
    }

    // Disconnect all slots
    void disconnectAll()
    {
        for (auto& slot : slots) {
            slot.connected = false;
        }
    }

    void emit(Args... args) const
    {
        // Remove disconnected slots first
        slots.erase(
            std::remove_if(slots.begin(), slots.end(), [](const Slot& slot) { return !slot.connected; }), slots.end());

        // Call remaining connected slots
        for (const auto& slot : slots) {
            slot.func(args...);
        }
    }

    void operator()(Args... args) const { emit(args...); }

    size_t connectionCount() const
    {
        return std::count_if(slots.begin(), slots.end(), [](const Slot& slot) { return slot.connected; });
    }
};
