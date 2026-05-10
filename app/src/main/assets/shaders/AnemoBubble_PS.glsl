#version 300 es
precision highp float;

uniform float _Time;

in vec4 color;
in vec2 uv;
out vec4 fnl;

uniform sampler2D utilTex;

#define PI 3.14159265359

vec2 Polar(vec2 uv) {
    uv = (uv * 2.f) - 1.f;
    vec2 result = vec2(0.f);
    result.x = length(uv);
    result.y = (atan(uv.y, uv.x) + PI) / (2.f * PI);
    return result;
}

vec2 Rotate(vec2 p, float a)
{
    float s = sin(a);
    float c = cos(a);
    return mat2(c, -s, s, c) * p;
}

void main() {
    vec2 polarCoord = Polar(uv);

    vec3 green = vec3(.106f, 1.f, .473f);

    float rimWind = texture(utilTex, polarCoord * vec2(1.f, .4f) + _Time * vec2(.2f, .3f)).g;
    rimWind = step(rimWind, polarCoord.x);
    fnl.rgb = vec3(0.0f);
    fnl.a = rimWind;
    return;
}
