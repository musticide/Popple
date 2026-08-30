#pragma once
#include "Entity.h"
#include "Singleton.h"
#include "Tween.h"
#include "raylib.h" // Assuming Raylib for Vector2, Vector3, Color
#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

class TweenManager : public Singleton<TweenManager>{
  private:
    // Separate vectors ensure O(1) cache locality for each type update
    std::vector<Tween<float>> floatTweens;
    std::vector<Tween<Vector2>> vec2Tweens;
    std::vector<Tween<Vector3>> vec3Tweens;
    std::vector<Tween<Color>> colorTweens;

    // A generic update loop to prevent code duplication
    template <typename T>
    void UpdateTweens(std::vector<Tween<T>>& tweens, float dt) {
        for (int i = static_cast<int>(tweens.size()) - 1; i >= 0; --i) {
            Tween<T>& t = tweens[i];

            t.elapsed += dt;
            float progress = t.elapsed / t.duration;
            if (progress > 1.0f) progress = 1.0f;

            // Apply easing function before Lerping
            float easedProgress = t.easeFunc(progress);
            T currentValue      = Lerp<T>(t.start, t.end, easedProgress);

            if (t.target) {
                *(t.target) = currentValue;
            }

            if (t.onUpdateFn) {
                t.onUpdateFn(currentValue);
            }

            if (progress >= 1.0f) {
                if (t.onCompletedFn) t.onCompletedFn();

                // O(1) Removal
                if (i != tweens.size() - 1) {
                    tweens[i] = std::move(tweens.back());
                }
                tweens.pop_back();
            }
        }
    }

  public:
    TweenManager(){}
    // Overloaded 'To' functions acting as factories
    Tween<float>& To(float* target, float start, float end, float duration) {
        floatTweens.emplace_back(target, start, end, duration);
        return floatTweens.back();
    }

    Tween<Vector2>& To(Vector2* target, Vector2 start, Vector2 end, float duration) {
        vec2Tweens.emplace_back(target, start, end, duration);
        return vec2Tweens.back();
    }

    Tween<Vector3>& To(Vector3* target, Vector3 start, Vector3 end, float duration) {
        vec3Tweens.emplace_back(target, start, end, duration);
        return vec3Tweens.back();
    }

    Tween<Color>& To(Color* target, Color start, Color end, float duration) {
        colorTweens.emplace_back(target, start, end, duration);
        return colorTweens.back();
    }

    void Update(float dt) {
        UpdateTweens(floatTweens, dt);
        UpdateTweens(vec2Tweens, dt);
        UpdateTweens(vec3Tweens, dt);
        UpdateTweens(colorTweens, dt);
    }
};
