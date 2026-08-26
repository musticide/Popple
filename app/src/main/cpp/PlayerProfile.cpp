#pragma once

#include "PlayerProfile.h"
#include "Globals.h"
#include "Log.h"
#include "firebase.h"
#include <cstdint>
#include <firebase/database/database_reference.h>
#include <firebase/variant.h>
#include <map>
#include <string>


firebase::Variant GetFormattedPlayerProfile() {
    std::map<firebase::Variant, firebase::Variant> rootUserNode;
    rootUserNode[PlayerProfile.username.key]            = PlayerProfile.username.value;
    rootUserNode[PlayerProfile.deviceId.key]            = PlayerProfile.deviceId.value;
    rootUserNode[PlayerProfile.highestScore.key]        = PlayerProfile.highestScore.value;
    rootUserNode[PlayerProfile.longestTimeSurvived.key] = PlayerProfile.longestTimeSurvived.value;
    rootUserNode[PlayerProfile.highestLevelCleared.key] = PlayerProfile.highestLevelCleared.value;
    rootUserNode[PlayerProfile.platform.key]            = PlayerProfile.platform.value;
    rootUserNode[PlayerProfile.appVersion.key]          = PlayerProfile.appVersion.value;
    rootUserNode[PlayerProfile.lastUpdated.key]         = PlayerProfile.lastUpdated.value;

    return firebase::Variant(rootUserNode);
}

void SyncLevelToFirebase(int levelNumber) {
    using namespace firebase;
    const LevelStats& stats = PlayerProfile.levelsData.value[levelNumber - 1];
    if (g_firebaseDB == nullptr) {
        LOGE("Level Sync Failed. Could not find fireDB");
        return;
    }
    if (g_UserId.empty()) {
        LOGE("Level Sync Failed. User id is empty");
        return;
    }

    // Target path: users/<uid>/levels/<levelNumber>
    std::string path = "users/" + g_UserId + "/levels/" + std::to_string(levelNumber);
    firebase::database::DatabaseReference levelRef = g_firebaseDB->GetReference(path.c_str());

    std::map<std::string, firebase::Variant> levelData;
    levelData["rating"] = stats.rating;
    levelData["score"]  = stats.score;
    levelData["time"]   = stats.time;

    // UpdateChildren only modifies the fields for this single level
    levelRef.UpdateChildren(levelData).OnCompletion([levelNumber](const firebase::Future<void>& future) {
        if (future.error() == firebase::database::kErrorNone) {
            LOGI("Level %d sync successful.", levelNumber);
        } else {
            LOGE("Level Sync Failed: %s", future.error_message());
        }
    });

    if (stats.rating > 0 && levelNumber > PlayerProfile.highestLevelCleared.value) {
        PlayerProfile.highestLevelCleared.value = levelNumber;
        LOGI("PLAYER_PROFILE: Highest level cleared %d", PlayerProfile.highestLevelCleared.value);

        firebase::database::DatabaseReference highestLevelCleardRef =
            g_firebaseDB->GetReference("users").Child(g_UserId).Child(PlayerProfile.highestLevelCleared.key);

        highestLevelCleardRef.SetValue(PlayerProfile.highestLevelCleared.value).OnCompletion([](const Future<void>& future) {
            if (future.error() == firebase::database::kErrorNone) {
                LOGI("Highest level Saved.");
            } else {
                LOGE("Highest level sync Failed: %s", future.error_message());
            }
        });
    }
}

// --- PULL ALL LEVELS ON BOOT / LOGIN ---
void LoadLevelsFromFirebase() {
    if (g_firebaseDB == nullptr || g_UserId.empty()) return;

    firebase::database::DatabaseReference levelsRef =
        g_firebaseDB->GetReference("users").Child(g_UserId).Child(PlayerProfile.levelsData.key.c_str());

    levelsRef.GetValue().OnCompletion([](const firebase::Future<firebase::database::DataSnapshot>& future) {
        if (future.error() != firebase::database::kErrorNone) {
            LOGE("Failed to fetch levels: %s", future.error_message());
            return;
        }

        const firebase::database::DataSnapshot* snapshot = future.result();
        if (snapshot->exists()) {
            if (snapshot->value().is_map()) {

                // Clear existing local levels to prevent stale data
                PlayerProfile.levelsData.value.clear();

                std::map<firebase::Variant, firebase::Variant> remoteLevels = snapshot->value().map();

                for (const auto& [levelKeyVariant, statsVariant] : remoteLevels) {
                    if (!statsVariant.is_map()) continue;

                    int levelNumber = std::stoi(levelKeyVariant.string_value());
                    std::map<firebase::Variant, firebase::Variant> statsMap = statsVariant.map();

                    LevelStats stats;
                    if (statsMap.count("rating")) stats.rating = statsMap["rating"].int64_value();
                    if (statsMap.count("score")) stats.score = statsMap["score"].int64_value();
                    if (statsMap.count("time")) stats.time = statsMap["time"].int64_value();

                    // Insert into the local sparse map
                    PlayerProfile.levelsData.value[levelNumber - 1] = stats;
                }

                LOGI("Loaded %d level records from Firebase.", (int)PlayerProfile.levelsData.value.size());
            } else if (snapshot->value().is_vector()) {
                // Handle as Vector (Array) - Firebase coerces numeric keys to this!
                std::vector<firebase::Variant> remoteLevels = snapshot->value().vector();

                for (int i = 0; i < remoteLevels.size(); ++i) {
                    const firebase::Variant& statsVariant = remoteLevels[i];

                    // Skip null variants (e.g., index 0 if your levels start at 1)
                    if (!statsVariant.is_map()) continue;

                    int levelNumber = i; // The vector index is the level number
                    std::map<firebase::Variant, firebase::Variant> statsMap = statsVariant.map();

                    LevelStats stats;
                    if (statsMap.count("rating")) stats.rating = statsMap["rating"].int64_value();
                    if (statsMap.count("score")) stats.score = statsMap["score"].int64_value();
                    if (statsMap.count("time")) stats.time = statsMap["time"].int64_value();

                    PlayerProfile.levelsData.value[levelNumber - 1] = stats;
                }
                LOGI("Loaded %d level records from Firebase (Vector).", (int)PlayerProfile.levelsData.value.size());
            } else {

                LOGI("No remote level data found. Fresh account.");
            }
        } else {

            LOGI("No remote level data found. Fresh account.");
        }
    });
}

void SyncMetaData() {
    if (g_firebaseDB == nullptr) {
        LOGE("Save Failed. Could not find fireDB");
        return;
    }
    if (g_UserId.empty()) {
        LOGE("Save Failed. User id is empty");
        return;
    }
    std::map<firebase::Variant, firebase::Variant> updates;
    updates[PlayerProfile.username.key]    = PlayerProfile.username.value;
    updates[PlayerProfile.deviceId.key]    = PlayerProfile.deviceId.value;
    updates[PlayerProfile.platform.key]    = PlayerProfile.platform.value;
    updates[PlayerProfile.appVersion.key]  = PlayerProfile.appVersion.value;
    updates[PlayerProfile.lastUpdated.key] = PlayerProfile.lastUpdated.value;

    firebase::database::DatabaseReference userRef = g_firebaseDB->GetReference("users").Child(g_UserId);

    userRef.UpdateChildren(updates).OnCompletion([](const firebase::Future<void>& future) {
        if (future.error() == firebase::database::kErrorNone) {
            LOGI("User metadata sync successful.");
        } else {
            LOGE("User metadata sync failed: %s", future.error_message());
        }
    });
}
