#version 300 es
precision mediump float;

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
    // rimWind *= polarCoord.x;
    rimWind = step(rimWind, polarCoord.x);

    float blackGlow = 1.f - clamp(polarCoord.x * 2.5f, 0.f, 1.f);

    vec2 windUVA = Rotate(uv, _Time * .5f);
    windUVA *= vec2(1.f, .1f);
    windUVA += _Time * vec2(.14f, .4f);

    float windA = texture(utilTex, windUVA).g;
    windA = step(windA, .35f);

    vec2 windUVB = Rotate(uv, -_Time * .5f);
    windUVB *= vec2(1.f, .1f);
    windUVB += _Time * vec2(.14f, .4f);

    float windB = texture(utilTex, windUVB).g;
    windB = step(windB, .35f);

    vec4 flow = texture(utilTex, uv);
    fnl = vec4(clamp(windA + windB, 0.f, 1.f));
    fnl.rgb *= green * 1.f;
    fnl.a += rimWind + blackGlow;
}
