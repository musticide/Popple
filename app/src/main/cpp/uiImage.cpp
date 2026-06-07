#include "uiImage.h"
#include "ResourceManager.h"
#include "raylib.h"
#include <raymob.h>

ui::Image::Image(const char* filepath, Rectangle rect, int fitType) : UIElement(rect, fitType) {
    m_Texture = ResourceManager::GetTexture(filepath);

    if (m_Texture && m_Texture->id != 0)
        drawRect = Rectangle{ 0, 0, (float)m_Texture->width, (float)m_Texture->height };
    else
        drawRect = Rectangle{ 0, 0, rect.width, rect.height };
}

ui::Image::~Image() {
}

void ui::Image::Draw() const {
    if (m_Texture && m_Texture->id != 0) {
        DrawTexturePro(*m_Texture, drawRect, fRect, Vector2{ 0, 0 }, 0.0f, tint);
    } else {
        DrawRectangleRec(fRect, PINK);
    }
}
