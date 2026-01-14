#version 300 es
precision mediump float;

// uniform mat4 matVP;
// uniform mat4 matGeo;
uniform mat4 mvp;
uniform float _Time;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec4 vertexColor;

uniform sampler2D utilTex;

out vec4 color;
out vec2 uv;

void main() {
    uv = vertexTexCoord;
    float noise = texture(utilTex, uv + vec2(0, _Time * .3f)).r;
    noise -= .5f;
    gl_Position = mvp * vec4(vertexPosition, 1);
}
