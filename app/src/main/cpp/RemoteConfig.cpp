#include "RemoteConfig.h"
#include "Log.h"
#include "firebase.h"

void FetchRemoteConfig() {
    if (g_firebaseDB == nullptr) return;

    firebase::database::DatabaseReference configRef = g_firebaseDB->GetReference("config");

    configRef.GetValue().OnCompletion([](const firebase::Future<firebase::database::DataSnapshot>& future) {
        if (future.error() != firebase::database::kErrorNone) {
            LOGE("Failed to fetch remote config: %s", future.error_message());
            return;
        }

        const firebase::database::DataSnapshot* snapshot = future.result();
        if (snapshot->exists() && snapshot->value().is_map()) {
            std::map<firebase::Variant, firebase::Variant> configMap = snapshot->value().map();

            for (const auto& [keyVariant, groupVariant] : configMap) {
                std::string key = keyVariant.string_value();

                // Check if this node is a LevelGroup (e.g., "LevelGroup0")
                if (key.find("LevelGroup") == 0 && groupVariant.is_map()) {

                    // Extract the index number from the end of the string
                    int index = std::stoi(key.substr(10));

                    std::map<firebase::Variant, firebase::Variant> groupMap = groupVariant.map();

                    // Helper lambda to safely parse numbers that Firebase might interpret as Ints or Doubles
                    auto getFloat = [](const firebase::Variant& variant) -> float {
                        if (variant.is_int64()) return (float)variant.int64_value();
                        if (variant.is_double()) return (float)variant.double_value();
                        return 0.0f;
                    };

                    // Populate Intervals and Chances
                    if (groupMap.count("StartSpawnInterval")) {
                        RemoteConfig.levelGroupConfig[index].startSpawnInterval =
                            getFloat(groupMap["StartSpawnInterval"]);
                    }
                    if (groupMap.count("MinSpawnInterval")) {
                        RemoteConfig.levelGroupConfig[index].minSpawnInterval =
                            getFloat(groupMap["MinSpawnInterval"]);
                    }
                    if (groupMap.count("PowerUpSpawnChance")) {
                        RemoteConfig.levelGroupConfig[index].powerUpSpawnChance =
                            (int)groupMap["PowerUpSpawnChance"].int64_value();
                    }

                    // Populate PowerUps array
                    if (groupMap.count("PowerUps") && groupMap["PowerUps"].is_map()) {
                        std::map<firebase::Variant, firebase::Variant> powerUpsMap = groupMap["PowerUps"].map();

                        // Default all to false first
                        for (int i = 0; i < (int)ElementType::COUNT; ++i) {
                            RemoteConfig.levelGroupConfig[index].availablePowerUps[i] = false;
                        }

                        // Map string keys to your ElementType enum
                        if (powerUpsMap.count("ANEMO")) {
                            RemoteConfig.levelGroupConfig[index].availablePowerUps[(int)ElementType::ANEMO] =
                                powerUpsMap["ANEMO"].bool_value();
                        }
                        if (powerUpsMap.count("CRYO")) {
                            RemoteConfig.levelGroupConfig[index].availablePowerUps[(int)ElementType::CRYO] =
                                powerUpsMap["CRYO"].bool_value();
                        }
                        if (powerUpsMap.count("ELECTRO")) {
                            RemoteConfig.levelGroupConfig[index].availablePowerUps[(int)ElementType::ELECTRO] =
                                powerUpsMap["ELECTRO"].bool_value();
                        }
                        if (powerUpsMap.count("SHADOW")) {
                            RemoteConfig.levelGroupConfig[index].availablePowerUps[(int)ElementType::SHADOW] = powerUpsMap["SHADOW"].bool_value();
                        }
                        // if (powerUpsMap.count("PYRO")) {
                        //     RemoteConfig.levelGroupConfig[index].availablePowerUps[(int)ElementType::PYRO] = powerUpsMap["PYRO"].bool_value();
                        // }
                    }
                }
            }
            LOGI("Remote config fetched successfully.");
        } else {
            LOGI("No remote config data found.");
        }
    });
}
