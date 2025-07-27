#include <GLES2/gl2.h>
#include <android/log.h>
#include <jni.h>
#include <sstream>
#include <stdio.h>
#include "Assets.h"
#include "ShaderParser.h"
#include <string>

#define LOG_TAG "Popple"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


GLuint shaderProgram = 0;
GLuint VBO = 0;
GLint positionAttribute = -1;

float triangleVertices[] = {
    0.0f, 0.5f, 0.0f, // Top vertex
    -0.5f, -0.5f, 0.0f, // Bottom left
    0.5f, -0.5f, 0.0f // Bottom right
};

GLuint CompileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint compiled;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);

        if (infoLength > 1) {
            char* infoLog = new char[infoLength];
            glGetShaderInfoLog(shader, infoLength, nullptr, infoLog);
            LOGE("Shader compile Error: %s", infoLog);
            delete[] infoLog;
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint CreateShaderProgram(ShaderProgramSource source)
{
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, source.VertexShaderSource);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, source.FragmentShaderSource);

    if (vertexShader == 0 || fragmentShader == 0)
        return 0;

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
        if (infoLength > 1) {
            char* infoLog = new char[infoLength];
            glGetProgramInfoLog(program, infoLength, 0, infoLog);
            LOGE("Program linking error: %s", infoLog);
            delete[] infoLog;
        }
        glDeleteProgram(program);
        return 0;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

extern "C" {
JNIEXPORT void JNICALL Java_com_example_bubbleshooter_GameRenderer_initializeOpenGL(JNIEnv* env, jobject thiz, jobject assetManager)
{
    LOGI("OpenGL Initializing...");

    g_assetManager = AAssetManager_fromJava(env, assetManager);
    if(!g_assetManager)
    {
        LOGE("Failed to Load Asset Manager");
        return;
    }

    shaderProgram = CreateShaderProgram(ParseShader("Shaders/BasicShader.glsl"));
    //WARN: possible memory leak after parse shader need to call Free
    //will do in the abstracted class

    if(shaderProgram == 0)
    {
        LOGE("Failed to create shader program");
        return;
    }

    positionAttribute = glGetAttribLocation(shaderProgram, "a_position");
    if(positionAttribute == -1)
    {
        LOGE("Failed to get position attrib location");
        return;
    }

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);



    // Set clear color to blue (R, G, B, A)
    glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    LOGI("OpenGL Initialization Complete");
}

JNIEXPORT void JNICALL Java_com_example_bubbleshooter_GameRenderer_resizeScreen(
    JNIEnv* env, jobject thiz, jint width, jint height)
{
    LOGI("Screen resized to: %d x %d", width, height);
    glViewport(0, 0, width, height);
}

JNIEXPORT void JNICALL Java_com_example_bubbleshooter_GameRenderer_renderFrame(JNIEnv* env, jobject thiz)
{
    // Clear the screen with our blue color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnableVertexAttribArray(positionAttribute);
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(positionAttribute);

    GLenum error = glGetError();
    if(error!= GL_NO_ERROR)
    {
        LOGE("OpenGl error: 0x%x", error);
    }
}
}
