#include "Button.h"
#include "input.h"
#include <raymob.h>

Button::~Button()
{
    if (m_Texture.id != 0)
        UnloadTexture(m_Texture);
}
Button::Button(const char* filepath, Color color)
    : m_Color(color)
{
    m_Texture = LoadTexture(filepath);
    if (m_Texture.id != 0) {
        m_Rectangle.height = m_Texture.height;
        m_Rectangle.width = m_Texture.width;
        position = { -m_Texture.width / 2.0f, -m_Texture.height / 2.0f };
    } else {
        m_Rectangle.height = 200;
        m_Rectangle.width = 400;
        position = { -200.f, -100.f };
    }
    m_Rectangle.x = 0;
    m_Rectangle.y = 0;
}

Button::Button(const char* filepath, Color color, Vector2 position)
    : m_Color(color)
{
    Vector2 fPos;
    m_Texture = LoadTexture(filepath);
    if (m_Texture.id != 0) {
        m_Rectangle.height = m_Texture.height;
        m_Rectangle.width = m_Texture.width;
        this->position.x = position.x - m_Texture.width / 2.0f;
        this->position.y = position.y - m_Texture.height / 2.0f;
    } else {
        m_Rectangle.height = 200;
        m_Rectangle.width = 400;
        this->position = { -200.f, -100.f };
    }
    m_Rectangle.x = 0;
    m_Rectangle.y = 0;
}
Button::Button(const char* filepath)
    : m_Color(WHITE)
{
    m_Texture = LoadTexture(filepath);
    if (m_Texture.id != 0) {
        m_Rectangle.height = m_Texture.height;
        m_Rectangle.width = m_Texture.width;
        position = { -m_Texture.width / 2.0f, -m_Texture.height / 2.0f };
    } else {
        m_Rectangle.height = 200;
        m_Rectangle.width = 400;
        position = { -200.f, -100.f };
    }
    m_Rectangle.x = 0;
    m_Rectangle.y = 0;
}

void Button::Draw() const
{
    if (m_Texture.id != 0)
        DrawTextureRec(m_Texture, m_Rectangle, position, m_Color);
    else
        DrawRectangleRec(m_Rectangle, PINK);
}

void Button::Update(float dT)
{
    for (int i = 0; i < GetTouchPointCount(); i++) {
        if (CheckCollisionPointRec(Input::GetTouchPositionWS(i), m_Rectangle)) {
            onClick();
            break;
        }
    }
}

void Button::AddOnClickListener(std::function<void()> func) { onClick.connect(func); }
