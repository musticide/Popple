#pragma once
#include "DrawableEntity.h"
#include "raylib.h"
#include "raymath.h"
#include "uiCanvas.h"
#include <vector>

namespace ui {

enum FitType {
    FIXED_W   = (1 << 0),
    FIXED_H   = (1 << 1),
    STRETCH_W = (1 << 2),
    STRETCH_H = (1 << 3)
};

class UIElement : public DrawableEntity {
  public:
    UIElement(Scene* parentScene, Rectangle rect, int fitType = STRETCH_W | STRETCH_H);
    UIElement(UIElement&&)                 = default;
    UIElement(const UIElement&)            = default;
    UIElement& operator=(UIElement&&)      = default;
    UIElement& operator=(const UIElement&) = default;
    ~UIElement();

    bool preserveRatio = true;
    static Vector2 s_ScreenScale;

    Rectangle baseRect, fRect, drawRect;
    int fitType;


    ui::Canvas* parentCanvas;

    UIElement* parent = nullptr;
    std::vector<UIElement*> children;

    template <typename T, typename... Args>
    std::unique_ptr<T> CreateChild(bool active, Args&&... args) {

        auto child = parentCanvas->CreateElement<T>(active, std::forward<Args>(args)...);

        if (child.get()) {
            child->parent = this;
            child->UpdateFinalRect();
            this->children.push_back(child.get());
        }

        return std::move(child);
    }

    void UpdateFinalRect();

    void SetAnchor(Vector2 anchor) {
        this->anchor = anchor;
        UpdateFinalRect();
    }

    void SetPivot(Vector2 pivot) {
        this->pivot = pivot;
        UpdateFinalRect();
    }

    void SetScale(float scale) {
        this->scale = scale;
        UpdateFinalRect();
    }

    float GetScale() {
        return scale;
    }

    void SetRotation(float rotation) {
        this->rotation = rotation;
        UpdateFinalRect();
    }

  private:
    float scale    = 1.f;
    float rotation = 0.f;
    Vector2 pivot  = { .5f, .5f };
    Vector2 anchor = { .5f, .5f };
};

} // namespace ui
