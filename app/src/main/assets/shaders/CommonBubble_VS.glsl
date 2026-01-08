#version 300 es
precision mediump float;

// uniform mat4 matVP;
// uniform mat4 matGeo;
uniform mat4 mvp;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec4 vertexColor;

out vec2 uv;

void main() {
    uv = vertexTexCoord;
    gl_Position = mvp * vec4(vertexPosition, 1);
}
