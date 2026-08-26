#pragma once
#include <firebase/variant.h>
#include <string>
#include <map>

inline std::string g_UserId = "";

template <typename T>
struct PlayerData {
    T value;
    std::string key;
    PlayerData<T>(std::string key, T value) {
        this->value = value;
        this->key   = key;
    }
};

struct LevelStats {
    int rating = 0;
    int score = 0;
    int time  = 0; // in seconds or milliseconds
};

using LevelsStats = std::map<int, LevelStats>;


///NOTE: Need to call Sync to Upload data to firebase
inline struct PlayerProfile {
    // User data
    PlayerData<std::string> deviceId = PlayerData<std::string>("device_id", "");
    PlayerData<std::string> username = PlayerData<std::string>("username", "");

    // Stats
    PlayerData<int> highestScore = PlayerData<int>("highest_score", 0);
    /// In Seconds
    PlayerData<int> longestTimeSurvived = PlayerData<int>("longest_time_survived", 0);
    /// Actual level number
    PlayerData<int> highestLevelCleared = PlayerData<int>("highest_level_cleared", 0);

    ///On Index
    PlayerData<LevelsStats> levelsData {"levels", LevelsStats{}};

    // metadata
    PlayerData<std::string> platform   = PlayerData<std::string>("platform", "Android");
    PlayerData<std::string> appVersion = PlayerData<std::string>("app_version", "0.0.0");
    PlayerData<int64_t> lastUpdated    = PlayerData<int64_t>("last_updated", 0);
} PlayerProfile;

firebase::Variant GetFormattedPlayerProfile();

void LoadLevelsFromFirebase();
void SyncLevelToFirebase(int levelNumber);
void SyncMetaData();
