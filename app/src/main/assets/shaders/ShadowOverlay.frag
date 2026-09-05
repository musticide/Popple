#version 300 es
precision highp float;

uniform float _Time;

in vec4 color;
in vec2 uv;
out vec4 fnl;

#define PI 3.14159265359

vec2 Polar(vec2 uv) {
    uv = (uv * 2.f) - 1.f;
    vec2 result = vec2(0.f);
    result.x = length(uv);
    result.y = (atan(uv.y, uv.x) + PI) / (2.f * PI);
    return result;
}

void main() {
    vec2 polarCoord = Polar(uv);

    fnl.rgb = vec3(0.0f);
    fnl.a = min(polarCoord.x * 2.3f, 1.f);
    return;
}
