#pragma once
#include "Log.h"
#include "Singleton.h"
#include "raylib.h"
#include <cassert>
#include <cstddef>
#include <raymob.h>

class Input : public Singleton<Input>{
private:
    static Vector2 m_LastPolledTouchPos;
    Camera3D* m_Camera3D;
    Camera2D* m_Camera2D;

public:
    static Vector3 GetTouchPositionWS(int index = 0)
    {
        Ray r = GetScreenToWorldRay(GetTouchPosition(index), *Get().m_Camera3D);
        return r.position;
    }

    static Vector2 GetTouchPositionCS(int index = 0)
    {
        return GetScreenToWorld2D(GetTouchPosition(index), *Get().m_Camera2D);
    }
    static int GetTouchPointCount() { return GetTouchPointCount(); }

    Input(Camera3D* cam3d, Camera2D* cam2d);
    ~Input();
private:

};
