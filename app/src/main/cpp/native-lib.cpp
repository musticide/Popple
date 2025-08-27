#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include <GLES3/gl3.h>
// #include <GLES3/gl32.h>
#include <android/log.h>
#include <jni.h>
#include <sstream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Assets.h"
#include "Log.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

AAssetManager* g_assetManager = nullptr;

GLuint shaderProgram = 0;
GLuint VBO = 0;
GLuint VAO = 0;
GLint positionAttribute = -1;

float triangleVertices[] = {
    0.0f, 0.5f, 0.0f, // Top vertex
    -0.5f, -0.5f, 0.0f, // Bottom left
    0.5f, -0.5f, 0.0f // Bottom right
};

float quadVertices[] = {
    // Positions        // Texture coordinates
    -0.5f,  0.5f, 0.0f,    0.0f, 0.0f,  // Top left
     0.5f,  0.5f, 0.0f,    1.0f, 0.0f,  // Top right
     0.5f, -0.5f, 0.0f,    1.0f, 1.0f,  // Bottom right
    -0.5f, -0.5f, 0.0f,    0.0f, 1.0f   // Bottom left
};

/* float quadVertices[] = {
    // Positions
    -0.5f,  0.5f, 0.0f,  // Top left
     0.5f,  0.5f, 0.0f,  // Top right
     0.5f, -0.5f, 0.0f,  // Bottom right
    -0.5f, -0.5f, 0.0f,  // Bottom left
}; */

// Indices for quad (two triangles)
unsigned int quadIndices[] = {
    0, 1, 2,  // First triangle
    2, 3, 0   // Second triangle
};


// extern Shader shader;
// extern VertexBuffer vb;
// extern IndexBuffer ib;
VertexArray va;
Shader shader = Shader();
VertexBuffer vb = VertexBuffer();
IndexBuffer ib = IndexBuffer();
Texture texture;


extern "C" {
JNIEXPORT void JNICALL Java_com_example_bubbleshooter_GameRenderer_initializeOpenGL(
    JNIEnv* env, jobject thiz, jobject assetManager)
{
    LOGI("OpenGL Initializing...");

    // g_assetManager = AAssetManager_fromJava(env, assetManager);
    // if (!g_assetmanager) {

    if (!AndroidAsset::InitManager(env, assetManager)) {
        LOGE("Failed to Load Asset Manager");
        return;
    }

    shader = Shader("Shaders/BasicShader.glsl");

    texture = Texture("bubble.png");
    texture.Bind(0);
    shader.SetUniform("u_Texture", 0);


    vb = VertexBuffer(quadVertices, sizeof(quadVertices));

    va = VertexArray(0);
    VertexBufferLayout layout;
    layout.Push<float>(3);//positions
    layout.Push<float>(2);//texCoords

    va.AddBuffer(vb, layout);

    ib = IndexBuffer(quadIndices, 6);

    va.Unbind();
    glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    //WARN: Check when to delete these objects
    //  va.Delete();
    // shader.Delete();
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    vb.Delete();
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

    shader.Bind();


    va.Bind();
    ib.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);// nullptr cuz indices are bound at ib.Bind()

    va.Unbind();


    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        LOGE("OpenGl error: 0x%x", error);
    }

}
}
