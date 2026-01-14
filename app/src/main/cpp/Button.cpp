#include "Button.h"
#include "DrawableEntity.h"
#include "Entity.h"
#include "Log.h"
#include "ResourceManager.h"
#include "input.h"
#include "raylib.h"
#include <raymob.h>

Button::~Button() { }

Button::Button(const char* filepath, Color color, Vector2 centerPosition)
    : DrawableEntity(RenderQueue::UI)
    , m_Filepath(filepath)
    , m_Color(color)
{
    m_Texture = ResourceManager::GetTexture(m_Filepath);

    if (m_Texture && m_Texture->id != 0) {
        m_Rectangle.width = (float)m_Texture->width;
        m_Rectangle.height = (float)m_Texture->height;

        // Centered placement
        m_Rectangle.x = centerPosition.x - m_Rectangle.width * 0.5f;
        m_Rectangle.y = centerPosition.y - m_Rectangle.height * 0.5f;

        m_RecDraw = { 0.0f, 0.0f, (float)m_Texture->width, (float)m_Texture->height };
    } else {
        m_Rectangle = { 0, 0, 400, 200 };
    }
}

void Button::Draw() const
{
    if (m_Texture && m_Texture->id != 0) {
        DrawTextureRec(*m_Texture, m_RecDraw, { m_Rectangle.x, m_Rectangle.y }, m_Color);
    } else {
        DrawRectangleRec(m_Rectangle, PINK);
    }
}

void Button::Update(float dT)
{
    for (int i = 0; i < GetTouchPointCount(); i++) {
        if (CheckCollisionPointRec(Input::GetTouchPositionCS(i), m_Rectangle)) {
            onClick();
            break;
        }
    }
}

void Button::AddOnClickListener(std::function<void()> func) { onClick.connect(func); }
