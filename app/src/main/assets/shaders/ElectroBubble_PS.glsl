#version 300 es
precision mediump float;

uniform highp float _Time;

in vec4 color;
in vec2 uv;
out vec4 fnl;

uniform sampler2D utilTex;
uniform int _Blink;

#define PI 3.14159265359
#define NOISE_SPEED 0.8f
#define LIGHTNING_UPTIME 0.2f
#define FLICKER_SPEED 2.0f
#define LINE_AMOUNT 50.0f
#define LINE_SPEED 10.0f
#define INTENSITY 10.0f
#define BLINK_SPEED 8.0f

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
    const vec3 purple = vec3(.185f, .06f, 1.f);
    float circleSDF = length(uv - 0.5f);

    float lines = sin(circleSDF * LINE_AMOUNT - _Time * LINE_SPEED);

    // fnl.rgb = (purple + vec3(0.1f)) * INTENSITY;
    fnl.rgb = purple * INTENSITY;
    fnl.a = lines;

    float blink = _Blink > 0 ? 1.0f - (fract(_Time * BLINK_SPEED)) : 1.0f;

    fnl.a *= blink;

    return;
}
