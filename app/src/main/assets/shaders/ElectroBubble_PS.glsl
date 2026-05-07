#version 300 es
precision mediump float;

uniform highp float _Time;

in vec4 color;
in vec2 uv;
out vec4 fnl;

uniform sampler2D utilTex;
uniform bool _Blink;

#define PI 3.14159265359
#define NOISE_SPEED 0.8f
#define LIGHTNING_UPTIME 0.2f
#define FLICKER_SPEED 2.0f
#define LINE_AMOUNT 50.0f
#define LINE_SPEED 10.0f
#define INTENSITY 2.0f
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

    fnl.rgb = (purple + vec3(0.1f)) * INTENSITY;
    fnl.a = lines;

    float blink = _Blink ? 1.0f - (fract(_Time * BLINK_SPEED)) : 1.0f;

    fnl.a *= blink;

    return;
    vec2 polarCoord = Polar(uv);

    vec3 color = vec3(.185f, .06f, 1.f);

    vec2 superPerlinUV = polarCoord;
    superPerlinUV.x *= .4f;
    superPerlinUV.x += _Time * NOISE_SPEED;

    float superPerlin = texture(utilTex, superPerlinUV).g;

    float skipTime = floor(_Time * FLICKER_SPEED);
    float fracTime = fract(_Time * FLICKER_SPEED);

    vec2 lightningUVA = polarCoord + vec2(0, skipTime * .23f);
    lightningUVA += superPerlin * .2f;
    float show = (fracTime < LIGHTNING_UPTIME) ? 1.f : 0.f;
    lightningUVA.y *= 2.f;
    float lightningTexA = texture(utilTex, lightningUVA.yx).b * show;

    vec2 lightningUVB = polarCoord + vec2(0, skipTime * .43f);
    lightningUVB += superPerlin * .2f;
    show = (fracTime * 1.12f > 1.f - LIGHTNING_UPTIME) ? 1.f : 0.f;
    lightningUVB.y *= 2.f;
    float lightningTexB = texture(utilTex, lightningUVB.yx).b * show;

    float noise = texture(utilTex, uv * .5f + _Time * .5f).r;
    noise -= .5f;
    noise *= .18f;
    float rim = length(((uv + noise) * 2.f) - 1.f);

    fnl.rgb = color * 5.f;
    fnl.a = (lightningTexA + lightningTexB + rim);
}
