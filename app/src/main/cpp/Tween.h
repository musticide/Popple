#pragma once
#include "raylib.h" // Assuming Raylib for Vector2, Vector3, Color
#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

// --- Easing Equations ---
namespace Easing {
using Func = float (*)(float);

inline float Linear(float t) {
    return t;
}
inline float QuadIn(float t) {
    return t * t;
}
inline float QuadOut(float t) {
    return t * (2.0f - t);
}
inline float QuadInOut(float t) {
    return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
}
inline float CubicIn(float t) {
    return t * t * t;
}
inline float CubicOut(float t) {
    float f = t - 1.0f;
    return f * f * f + 1.0f;
}
} // namespace Easing

// --- Type-Specific Interpolation (Lerp) ---
template <typename T>
inline T Lerp(const T& start, const T& end, float t);

template <>
inline float Lerp(const float& start, const float& end, float t) {
    return start + (end - start) * t;
}

template <>
inline Vector2 Lerp(const Vector2& start, const Vector2& end, float t) {
    return { start.x + (end.x - start.x) * t, start.y + (end.y - start.y) * t };
}

template <>
inline Vector3 Lerp(const Vector3& start, const Vector3& end, float t) {
    return { start.x + (end.x - start.x) * t, start.y + (end.y - start.y) * t, start.z + (end.z - start.z) * t };
}

template <>
inline Color Lerp(const Color& start, const Color& end, float t) {
    return { static_cast<unsigned char>(start.r + (end.r - start.r) * t),
        static_cast<unsigned char>(start.g + (end.g - start.g) * t),
        static_cast<unsigned char>(start.b + (end.b - start.b) * t),
        static_cast<unsigned char>(start.a + (end.a - start.a) * t) };
}
template <typename T>
class Tween {
    friend class TweenManager;

  private:
    T* target = nullptr;
    T start;
    T end;
    float duration        = 0.0f;
    float elapsed         = 0.0f;
    Easing::Func easeFunc = Easing::Linear;

    std::function<void(T)> onUpdateFn   = nullptr;
    std::function<void()> onCompletedFn = nullptr;

  public:
    Tween(T* t, T s, T e, float d)
    : target(t)
    , start(s)
    , end(e)
    , duration(d) {
    }

    // Chainable Builders
    Tween& SetEasing(Easing::Func fn) {
        easeFunc = fn;
        return *this;
    }

    Tween& OnUpdate(std::function<void(T)> fn) {
        onUpdateFn = std::move(fn);
        return *this;
    }

    Tween& OnCompleted(std::function<void()> fn) {
        onCompletedFn = std::move(fn);
        return *this;
    }
};
