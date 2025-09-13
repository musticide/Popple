#pragma once

#include "raylib.h"
#include <raymob.h>

class Bubble {
public:
    Bubble();
    ~Bubble();

    Vector2 position;
    float rotation;
    float radius;
    Color color;
    Vector2 moveSpeed;

    void Update();

    void Move();

    void Draw() const;

    void SetActive(bool active);

    bool IsActive() { return m_IsActive; }

    bool IsPointInBubble(Vector2 point) const;

private:
    bool m_IsActive = true;
};
