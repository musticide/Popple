#include "firebase.h"
#include "raylib.h"
#include "raymob.h"
#include "Log.h"
#include <firebase/app.h>
#include <firebase/database.h>

firebase::App* g_firebaseApp = nullptr;
firebase::database::Database* g_firebaseDB = nullptr;

void InitializeFirebaseCore() {
    struct android_app* native_app = GetAndroidApp();
    if (!native_app) {
        LOGE("FIREBASE: Failed to retrieve native android_app state from Raymob.");
        return;
    }

    JNIEnv* jni_env = nullptr;
    native_app->activity->vm->AttachCurrentThread(&jni_env, nullptr);

    if (!jni_env) {
        LOGE("FIREBASE: Failed to attach current thread to JVM.");
        return;
    }

    // 3. Spin up the core Firebase App container
    LOGI("FIREBASE: Initializing core C++ SDK instance...");
    g_firebaseApp = firebase::App::Create(jni_env, native_app->activity->clazz);

    if (g_firebaseApp != nullptr) {
        LOGI("FIREBASE: Core SDK initialization successful!");
        firebase::InitResult dbInitResult;
        g_firebaseDB = firebase::database::Database::GetInstance(g_firebaseApp, & dbInitResult);
        if(dbInitResult == firebase::kInitResultSuccess){
            LOGI("FIREBASE: Realtime DB Module is ready");
        }
        else {
        LOGE("FIREBASE: Failed to initialize Realtime DB");
        }
    } else {
        LOGE("FIREBASE: Failed to create core Firebase App instance.");
    }
}

void CleanupFirebase() {
    if (g_firebaseApp != nullptr) {
        delete g_firebaseApp;
        g_firebaseApp = nullptr;
    }
}
