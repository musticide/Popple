#include "Button.h"
#include "Entity.h"
#include "Log.h"
#include "input.h"
#include <raymob.h>
#include "ResourceManager.h"

Button::~Button()
{
}
Button::Button(const char* filepath, Color color)
    :m_Filepath(filepath), m_Color(color)
{
    m_Texture = ResourceManager::GetTexture(m_Filepath);
    if (m_Texture->id != 0) {
        m_Rectangle.height = m_Texture->height;
        m_Rectangle.width = m_Texture->width;
        position = { -m_Texture->width / 2.0f, -m_Texture->height / 2.0f };
    } else {
        m_Rectangle.height = 200;
        m_Rectangle.width = 400;
        position = { -200.f, -100.f };
    }
    m_Rectangle.x = 0;
    m_Rectangle.y = 0;
    SetRenderQueue(RenderQueue::UI);
}

Button::Button(const char* filepath, Color color, Vector2 position)
    :m_Filepath(filepath), m_Color(color)
{
    m_Texture = ResourceManager::GetTexture(m_Filepath);
    Vector2 fPos;
    if (m_Texture->id != 0) {
        m_Rectangle.height = m_Texture->height;
        m_Rectangle.width = m_Texture->width;
        this->position.x = position.x - m_Texture->width / 2.0f;
        this->position.y = position.y - m_Texture->height / 2.0f;
    } else {
        m_Rectangle.height = 200;
        m_Rectangle.width = 400;
        this->position = { -200.f, -100.f };
    }
    m_Rectangle.x = 0;
    m_Rectangle.y = 0;
    SetRenderQueue(RenderQueue::UI);
}
Button::Button(const char* filepath)
    :m_Filepath(filepath), m_Color(WHITE)
{
    m_Texture = ResourceManager::GetTexture(m_Filepath);
    if (m_Texture->id != 0) {
        m_Rectangle.height = m_Texture->height;
        m_Rectangle.width = m_Texture->width;
        position = { -m_Texture->width / 2.0f, -m_Texture->height / 2.0f };
    } else {
        m_Rectangle.height = 200;
        m_Rectangle.width = 400;
        position = { -200.f, -100.f };
    }
    m_Rectangle.x = 0;
    m_Rectangle.y = 0;
    SetRenderQueue(RenderQueue::UI);
}

void Button::Draw() const
{
    if (m_Texture->id != 0)
        DrawTextureRec(*m_Texture, m_Rectangle, position, m_Color);
    else
        DrawRectangleRec(m_Rectangle, PINK);
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
