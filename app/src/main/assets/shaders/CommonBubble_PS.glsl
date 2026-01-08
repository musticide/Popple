#version 300 es
precision mediump float;

in vec2 uv;

out vec4 fnl;

uniform float _Time;

uniform sampler2D texture0; //Util Tex
uniform sampler2D texture1; //Color tex

#define PI 3.14159265359
#define SWIRL_SPEED 0.07f
#define SWIRL_INTENSITY 0.25f
#define ROTATION_SPEED 0.3f
#define TRANSLATION_SPEED 0.1f

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

    vec2 flow = texture(texture0, uv * .5f + _Time * SWIRL_SPEED).rg;
    flow = (flow * 2.f) - 1.f;

    float circleSDF = length((uv * 2.f) - 1.f);
    flow *= circleSDF * SWIRL_INTENSITY;

    vec2 texUV = uv - .5f;
    texUV = Rotate(texUV, _Time * ROTATION_SPEED);
    texUV += .5f;
    texUV += flow;
    texUV += _Time * TRANSLATION_SPEED;

    fnl = texture(texture1, texUV);
}
