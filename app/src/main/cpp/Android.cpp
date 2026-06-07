#include "Android.h"
#include "raylib.h"
#include "raymob.h"

std::string GetAndroidDeviceID() {

    std::string device_id = "unknown_device";

    // 1. Retrieve the native android_app context from Raymob
    struct android_app* native_app = GetAndroidApp();
    if (!native_app) {
        TraceLog(LOG_ERROR, "JNI: Native app state from Raymob is null.");
        return device_id;
    }

    // 2. Attach the current thread to the JVM execution context
    JNIEnv* env        = nullptr;
    jint attach_result = native_app->activity->vm->AttachCurrentThread(&env, nullptr);
    if (attach_result != JNI_OK || env == nullptr) {
        TraceLog(LOG_ERROR, "JNI: Failed to attach thread to Java Virtual Machine.");
        return device_id;
    }

    // Wrap in a try/catch style exception check block to prevent raw NDK crashes
    if (env->ExceptionCheck()) env->ExceptionClear();

    // 3. Get the running Activity Instance (Context)
    jobject context_activity = native_app->activity->clazz;
    jclass context_class     = env->GetObjectClass(context_activity);

    // 4. Look up and call context.getContentResolver()
    jmethodID get_content_resolver =
        env->GetMethodID(context_class, "getContentResolver", "()Landroid/content/ContentResolver;");
    jobject content_resolver = env->CallObjectMethod(context_activity, get_content_resolver);

    // 5. Look up the Android provider Settings$Secure class framework
    jclass settings_secure_class = env->FindClass("android/provider/Settings$Secure");

    // 6. Locate the static getString(ContentResolver, String) method signature
    jmethodID get_string_method = env->GetStaticMethodID(
        settings_secure_class, "getString", "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;");

    // 7. Pass "android_id" string parameter to target the secure node identifier
    jstring android_id_constant = env->NewStringUTF("android_id");

    // 8. Execute the method to retrieve the Java String token
    jstring j_android_id = (jstring)env->CallStaticObjectMethod(
        settings_secure_class, get_string_method, content_resolver, android_id_constant);

    // 9. Safely extract and parse the Java String contents into a native std::string
    if (j_android_id != nullptr) {
        const char* native_chars = env->GetStringUTFChars(j_android_id, nullptr);
        if (native_chars != nullptr) {
            device_id = std::string(native_chars);
            env->ReleaseStringUTFChars(j_android_id, native_chars);
        }
        env->DeleteLocalRef(j_android_id);
    } else {
        TraceLog(LOG_WARNING, "JNI: Settings.Secure.ANDROID_ID returned null. Using fallback ID.");
    }

    // 10. Clean up local NDK reference instances
    env->DeleteLocalRef(android_id_constant);
    env->DeleteLocalRef(settings_secure_class);
    env->DeleteLocalRef(content_resolver);
    env->DeleteLocalRef(context_class);

    return device_id;
}

