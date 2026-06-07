#pragma once
#include "DrawableEntity.h"
#include "raylib.h"
#include <vector>
#include "uiCanvas.h"

namespace ui {

enum FitType {
    FIXED_H   = (1 << 0),
    FIXED_V   = (1 << 1),
    STRETCH_H = (1 << 2),
    STRETCH_V = (1 << 3)
};

class UIElement : public DrawableEntity {
  public:
    UIElement(Scene* parentScene, Rectangle rect, int fitType = STRETCH_H | STRETCH_V);
    UIElement(UIElement&&)                 = default;
    UIElement(const UIElement&)            = default;
    UIElement& operator=(UIElement&&)      = default;
    UIElement& operator=(const UIElement&) = default;
    ~UIElement();

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
            child->SetFinalRectangle();
            this->children.push_back(child.get());
        }

        return std::move(child);
    }

    void SetFinalRectangle();

  private:
};

} // namespace ui
