#pragma once
#include "Signal.h"
#include "raymob.h"

#define MAX_LEVELS 120

namespace Globals {

extern int screenWidth, screenHeight;
extern const int baseScreenWidth, baseScreenHeight;
extern Vector2 uiScale;

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
extern Signal<int> gameStateChanged;

enum State {
    MAIN_MENU        = (1 << 0),
    APP_PAUSED       = (1 << 1),
    GAMEPLAY_PAUSED  = (1 << 2),
    GAMEPLAY_RUNNING = (1 << 3),
    GAMEPLAY_ENDED   = (1 << 4)
};

extern int state;
void EnableState(int state);
void DisableState(int state);
bool IsStateValid(int state);

std::string GetStateNames(int state);
} // namespace Globals
