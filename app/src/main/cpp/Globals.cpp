#include "Globals.h"
#include "raylib.h"
#include <raymob.h>

int Globals::screenWidth  = 0;
int Globals::screenHeight = 0;

const int Globals::baseScreenWidth = 1080, Globals::baseScreenHeight = 2340;
Vector2 Globals::uiScale = { 0.f, 0.f };
bool Globals::gamePaused = false;

bool Globals::GameResults::gameCompleted;
bool Globals::GameResults::gameWon;
int Globals::GameResults::score;
int Globals::GameResults::health;
int Globals::GameResults::levelPlayed;
int Globals::GameResults::levelRating;
