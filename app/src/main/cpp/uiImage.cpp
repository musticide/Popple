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
        Color drawColor = ColorTint(tint, parentCanvas->canvasTint);

        // raylib rotates around `origin`, treating the dest rect's x/y as the
        // position of that origin point (not the top-left corner). To rotate
        // around this element's configured pivot rather than always its
        // top-left corner, we make the pivot's world position the dest
        // anchor and set origin to the pivot's offset from the rect's
        // top-left. With rotation 0 this still lands the image exactly on
        // fRect -- position correctness lives in the layout (fRect is fully
        // in screen units), not in the origin choice here.
        Vector2 pivot         = GetPivot();
        Vector2 pivotOffset   = { fRect.width * pivot.x, fRect.height * pivot.y };
        Vector2 pivotWorldPos = { fRect.x + pivotOffset.x, fRect.y + pivotOffset.y };

        if (isNPatch) {
            Rectangle dest = { pivotWorldPos.x, pivotWorldPos.y, fRect.width, fRect.height };
            DrawTextureNPatch(*m_Texture, nPatchInfo, dest, pivotOffset, GetRotation(), drawColor);
        } else if (isTiled) {

            float destTileWidth = fRect.width / (float)tileAmount;

            for (int i = 0; i < tileAmount; i++) {
                // Every strip shares the same world pivot so the whole tiled
                // group rotates together as one piece, rather than each strip
                // spinning around its own corner.
                Vector2 stripTopLeft = { fRect.x + (i * destTileWidth), fRect.y };
                Vector2 stripOrigin  = { pivotWorldPos.x - stripTopLeft.x, pivotWorldPos.y - stripTopLeft.y };
                Rectangle dest       = { pivotWorldPos.x, pivotWorldPos.y, destTileWidth, fRect.height };

                DrawTexturePro(*m_Texture, nPatchInfo.source, dest, stripOrigin, GetRotation(), drawColor);
            }
        } else {
            Rectangle dest = { pivotWorldPos.x, pivotWorldPos.y, fRect.width, fRect.height };
            DrawTexturePro(*m_Texture, nPatchInfo.source, dest, pivotOffset, GetRotation(), drawColor);
        }

    } else {
        DrawRectangleRec(fRect, PINK);
    }

    ui::UIElement::Draw();
}
