#include "input.h"
#include "raylib.h"


Input::Input(Camera3D* cam3d, Camera2D* cam2d)
: m_Camera3D(cam3d), m_Camera2D(cam2d){

}
Input::~Input(){}
