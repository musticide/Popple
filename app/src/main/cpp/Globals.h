#pragma once
#include "SceneManager.h"
#include <cstdint>

namespace Globals {

extern int screenWidth, screenHeight;
extern const int baseScreenWidth, baseScreenHeight;
extern Vector2 uiScale;
extern bool gamePaused;

void StartGame();

inline struct GameResults {
    static bool gameCompleted;
    static bool gameWon;
    static int score;
    static int health;
    static int levelRating;
    static int levelPlayed;
} GameResults;
} // namespace Globals
