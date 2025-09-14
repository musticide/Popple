#pragma once
// #include "raylib.h"
#include "raylib.h"
#include <cassert>
#include <cstddef>
#include <raymob.h>


class Input {
private:
    static Vector2 m_LastPolledTouchPos;
    Camera2D m_Camera;

    static Input* instance;
public:
    Input(Input &&) = delete;
    Input(const Input &) = delete;
    Input &operator=(Input &&) = delete;
    Input &operator=(const Input &) = delete;

    static void initialize(Camera2D cam);

    static Input& Get()
    {
        // assert(instance != && "Input not initialised");
        return *instance;
    }


    Vector2 GetTouchPositionWS(int index = 0)
    {
        return GetScreenToWorld2D(GetTouchPosition(index), m_Camera);
    }

    static int GetTouchPointCount()
{
        return GetTouchPointCount();
    }

private:
    Input();
    Input(Camera2D camera);

    ~Input();
};
