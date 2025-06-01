#include <jni.h>
#include <GLES2/gl2.h>
#include <android/log.h>

#define LOG_TAG "BubbleShooter"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
extern "C" {
    JNIEXPORT void JNICALL
    Java_com_example_bubbleshooter_GameRenderer_initializeOpenGL(JNIEnv *env, jobject thiz){
        LOGI("OpenGL initialized");

        // Set clear color to blue (R, G, B, A)
        glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
    }

    JNIEXPORT void JNICALL
    Java_com_example_bubbleshooter_GameRenderer_resizeScreen(JNIEnv *env, jobject thiz,
                                                             jint width, jint height) {
        LOGI("Screen resized to: %d x %d", width, height);
        glViewport(0, 0, width, height);
    }

    JNIEXPORT void JNICALL
    Java_com_example_bubbleshooter_GameRenderer_renderFrame(JNIEnv *env, jobject thiz) {
        // Clear the screen with our blue color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}
