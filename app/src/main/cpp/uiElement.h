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

#define ANCHOR_TOP_LEFT { 0.f, 0.f }
#define ANCHOR_TOP_CENTER { .5f, 0.f }
#define ANCHOR_TOP_RIGHT { 1.f, 0.f }

#define ANCHOR_MIDDLE_LEFT { 0.f, .5f }
#define ANCHOR_CENTER { .5f, .5f }
#define ANCHOR_MIDDLE_RIGHT { 1.f, .5f }

#define ANCHOR_BOTTOM_LEFT { 0.f, 1.f }
#define ANCHOR_BOTTOM_CENTER { .5f, 1.f }
#define ANCHOR_BOTTOM_RIGHT { 1.f, 1.f }

class UIElement : public DrawableEntity {
  public:
    UIElement(Scene* parentScene, Canvas* parentCanvas, Rectangle rect, int fitType = STRETCH_W | STRETCH_H);
    ~UIElement();

    bool preserveRatio = true;
    static Vector2 s_ScreenScale;

    Rectangle baseRect, fRect;
    int fitType;


    ui::Canvas* parentCanvas;

    UIElement* parent = nullptr;
    std::vector<UIElement*> children;
    Color tint = WHITE;

    // Mirrors fRect but excludes this element's own aniScale, so touch/click
    // hit-testing stays stable while a press/idle animation is playing.
    Rectangle raycastRect;

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

    virtual void UpdateFinalRect();

    void SetAnchor(Vector2 anchor) {
        this->anchor = anchor;
        UpdateFinalRect();
    }

    void SetPivot(Vector2 pivot) {
        this->pivot = pivot;
        UpdateFinalRect();
    }

    void SetScale(float scale) {
        this->scale = { scale, scale };
        UpdateFinalRect();
    }
    void SetScale(Vector2 scale) {
        this->scale = scale;
        UpdateFinalRect();
    }
    void SetAniScale(float scale) {
        this->aniScale = { scale, scale };
        UpdateFinalRect();
    }
    void SetAniScale(Vector2 scale) {
        aniScale = scale;
        UpdateFinalRect();
    }
    Vector2 const GetAniScale() const {
        return aniScale;
    }

    Vector2 const GetScale() const {
        return scale;
    }

    Vector2 const GetPivot() const {
        return pivot;
    }

    void SetRotation(float rotation) {
        this->rotation = rotation;
        UpdateFinalRect();
    }

    float const GetRotation() const {
        return rotation;
    }

    void OnEnable() override;
    void OnDisable() override;
    void Draw() const override;

  private:
    // Shared math for fRect/raycastRect. aniScaleOverride lets callers ask for
    // the rect with a different animation scale substituted in (e.g. {1,1} to
    // ignore the current press animation for raycastRect). Returns the rect in
    // pure layout space -- i.e. BEFORE canvas pan/zoom -- see ApplyCanvasTransform.
    Rectangle ComputeRect(Vector2 aniScaleOverride) const;

    // Applies this element's canvas pan/zoom to a pure-layout rect. Called
    // once per element (never recursively compounded), so canvas transforms
    // stay uniform across the whole tree regardless of nesting depth.
    Rectangle ApplyCanvasTransform(Rectangle rect) const;

    // Pure layout rect (no canvas pan/zoom applied). Children compute their
    // own layout relative to the parent's m_LayoutRect, not its final fRect,
    // so canvas transform can be applied once at the end without compounding.
    Rectangle m_LayoutRect;

    Vector2 scale    = { 1.f, 1.f };
    Vector2 aniScale = { 1.f, 1.f };
    float rotation   = 0.f;
    Vector2 pivot    = { .5f, .5f };
    Vector2 anchor   = { .5f, .5f };
};

} // namespace ui
