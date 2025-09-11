#include "bubble.h"
#include "raylib.h"
#include "raymath.h"

Bubble::Bubble() 
:position((Vector2){0,0}), rotation(0.0f), radius(1.0f),color(RAYWHITE){
}

Bubble::~Bubble() {
}

void Bubble::Move(){
    Vector2 moveDir = Vector2Normalize(Vector2Zero() - position);
    position += moveDir * moveSpeed;
}

void Bubble::Update(){
    Move();
}
