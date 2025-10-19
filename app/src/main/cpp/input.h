#pragma once
// #include "raylib.h"
#include "raylib.h"
#include <cassert>
#include <cstddef>
#include <raymob.h>

#include "popple.h"
class Input {
private:
    static Vector2 m_LastPolledTouchPos;
    Camera3D m_Camera3D;
    Camera2D m_Camera2D;

    static Input* instance;

public:
    Input(Input&&) = delete;
    Input(const Input&) = delete;
    Input& operator=(Input&&) = delete;
    Input& operator=(const Input&) = delete;

    static void initialize(Camera3D cam3d, Camera2D cam2d);

    static Input& Get()
    {
        // assert(instance != && "Input not initialised");
        return *instance;
    }

    static Vector3 GetTouchPositionWS(int index = 0)
    {
        Ray r = GetScreenToWorldRay(GetTouchPosition(index), Get().m_Camera3D);
        return r.position;
    }

    static Vector2 GetTouchPositionCS(int index = 0)
    {
        return GetScreenToWorld2D(GetTouchPosition(index), Get().m_Camera2D);
    }
    static int GetTouchPointCount() { return GetTouchPointCount(); }

private:
    Input();
    Input(Camera3D cam3d, Camera2D cam2d);

    ~Input();
};
