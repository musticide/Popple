#pragma once

#include "Entity.h"
#include "Signal.h"
#include <raymob.h>
#include <functional>

class Button : public Entity{
public:
    Button(const char* filepath, Color color, Vector2 position);
    Button(Button &&) = default;
    Button(const Button &) = default;
    Button &operator=(Button &&) = default;
    Button &operator=(const Button &) = default;
    ~Button();

    Vector2 position;

    void Update(float dT = 1.0f) override;

    void Draw() const override;

    void AddOnClickListener(std::function<void()> func);

private:
    std::shared_ptr<Texture2D> m_Texture;
    const char* m_Filepath;
    Rectangle m_Rectangle, m_RecDraw;
    Color m_Color;

   Signal<> onClick;
};
