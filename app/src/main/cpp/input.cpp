#include "input.h"
#include "raylib.h"

Input* Input::instance;

Input::Input(){}

Input::Input(Camera2D cam)
: m_Camera(cam){

}
Input::~Input(){}

void Input::initialize(Camera2D cam)
{
    instance = new Input(cam);
}
