#pragma once
#include "Log.h"
#include "Singleton.h"
#include "raylib.h"
#include <cassert>
#include <cmath>
#include <cstddef>
#include <raymob.h>

typedef struct {
    Vector2 start;
    Vector2 last;
    Vector2 velocity;
    float y;
    bool isDragging;
} TouchScroll;

class Input : public Singleton<Input> {
  private:
    Camera3D* m_Camera3D;
    Camera2D* m_Camera2D;

  public:
    static Vector3 GetTouchPositionWS(int index = 0) {
        Ray r = GetScreenToWorldRay(GetTouchPosition(index), *Get().m_Camera3D);
        return r.position;
    }

    static Vector2 GetTouchPositionCS(int index = 0) {
        return GetTouchPosition(index);
        // return GetScreenToWorld2D(GetTouchPosition(index), *Get().m_Camera2D);
    }
    static int GetTouchPointCount() {
        return GetTouchPointCount();
    }

    static bool IsTouchDown(int index = 0) {
        return IsMouseButtonDown(index);
    }

    static bool IsTouchReleased(int index = 0) {
        return IsMouseButtonReleased(index);
    }

    static void GetTouchScroll(TouchScroll* ts) {
        const float FRICTION  = 0.9f;
        const float MIN_SPEED = 0.5f;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            ts->start= GetTouchPosition(0);
            ts->last= ts->start;
            ts->velocity   = (Vector2){ 0, 0 };
            ts->isDragging = true;
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && ts->isDragging) {
            Vector2 cur   = GetTouchPosition(0);
            Vector2 delta = { cur.x - ts->last.x, cur.y - ts->last.y };

            ts->velocity = delta; // raw per-frame delta → becomes fling speed
            ts->y += delta.y;
            ts->last = cur;
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            ts->isDragging = false; // finger lifted → coast on velocity
        }

        if (!ts->isDragging) {
            ts->y += ts->velocity.y;
            ts->velocity.x *= FRICTION;
            ts->velocity.y *= FRICTION;
            if (fabsf(ts->velocity.y) < MIN_SPEED) ts->velocity.y = 0;
        }
    }

    Input(Camera3D* cam3d, Camera2D* cam2d);
    ~Input();

  private:
};
