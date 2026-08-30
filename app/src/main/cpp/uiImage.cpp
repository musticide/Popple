#include "uiImage.h"
#include "ResourceManager.h"
#include "raylib.h"
#include <raymath.h>
#include <raymob.h>


ui::Image::Image(Scene* parentScene, Canvas* parentCanvas, const char* filepath, Rectangle rect, int fitType, bool nPatch)
: UIElement(parentScene, parentCanvas, rect, fitType)
, isNPatch(nPatch) {
    m_Texture = ResourceManager::GetTexture(filepath);

    if (m_Texture && m_Texture->id != 0)
        nPatchInfo.source = Rectangle{ 0, 0, (float)m_Texture->width, (float)m_Texture->height };
    else
        nPatchInfo.source = Rectangle{ 0, 0, rect.width, rect.height };
}

ui::Image::~Image() {
}

void ui::Image::Draw() const {
    if (m_Texture && m_Texture->id != 0) {
        if (isNPatch)
            DrawTextureNPatch(
                *m_Texture, nPatchInfo, fRect, Vector2{ 0, 0 }, 0.0f, ColorTint(tint, parentCanvas->canvasTint));
        else
            DrawTexturePro(
                *m_Texture, nPatchInfo.source, fRect, Vector2{ 0, 0 }, 0.0f, ColorTint(tint, parentCanvas->canvasTint));

    } else {
        DrawRectangleRec(fRect, PINK);
    }

    ui::UIElement::Draw();
}
