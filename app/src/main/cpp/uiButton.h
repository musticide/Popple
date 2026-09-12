#pragma once
#include "Signal.h"
#include "raylib.h"
#include "uiCanvas.h"
#include "uiImage.h"
#include "uiText.h"
#include <memory>

namespace ui {
class Button : public ui::Image {
  public:
    Button(Scene* parentScene, Canvas* parentCanvas, const char* filepath, Rectangle rect, int fitType, bool nPatch = false);
    Button(Scene* parentScene,
        Canvas* parentCanvas,
        const char* filepath,
        Rectangle rect,
        std::string textContent,
        FontName font,
        int fitType,
        bool nPatch = false);
    ~Button();

    Signal<> onClick;

    void Update(float dT = 1.0f) override;
    bool clickable = true;

    std::unique_ptr<ui::Text> text = nullptr;

    void Start() override;
    void OnEnable() override;
    void OnDisable() override;

    int fontSize       = 72;
    Vector2 fontOffset = { 0, 0 };
    void SetTint(const Color& tint){
        this->tint = tint;
        if(text != nullptr)
        text->color = tint;

    }

  private:
    bool wasPressed = false;
    enum class PressState {
        None,
        PressedInside,
        PressedOutside
    };
    PressState state   = PressState::None;
    float pressedScale = 0.92f;
    FontName font;
    std::string textContent;
};
} // namespace ui
