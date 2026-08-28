#pragma once
#include "SceneManager.h"
#include "Signal.h"
#include "raymob.h"
#include <cstdint>

#define MAX_LEVELS 120

namespace Globals {

extern int screenWidth, screenHeight;
extern const int baseScreenWidth, baseScreenHeight;
extern Vector2 uiScale;
extern bool gamePaused;

inline struct GameResults {
    static bool gameCompleted;
    static bool gameWon;
    static int score;
    static int health;
    static int levelRating;
    static int levelPlayed;
} GameResults;

extern Callback onPauseCallBack;
extern Callback onResumeCallBack;
extern Signal<bool> gameStateChanged;

} // namespace Globals
