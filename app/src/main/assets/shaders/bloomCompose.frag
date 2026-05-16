#version 300 es
precision mediump float;

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec2 _ScreenSize;

// Output fragment color
out vec4 finalColor;

float luminance(vec3 rgb) {
    const vec3 W = vec3(0.2126, 0.7152, 0.0722);
    return dot(rgb, W);
}

void main()
{
    // Texel color fetching from texture sampler
    const float scale = 50.f;
    const float sdfSize = 1.5f;
    float ratio = _ScreenSize.y / _ScreenSize.x;
    vec2 uv = fragTexCoord;
    uv *= scale;
    vec4 source = texture(texture0, fragTexCoord);
    uv.y *= ratio;
    vec2 uvA = fract(uv);
    uvA -= .5f;
    uvA *= sdfSize;
    vec2 uvB = fract(uv - .5f);
    uvB -= .5f;
    uvB *= sdfSize;
    float halftone = min(length(uvA), length(uvB));

    halftone = halftone < luminance(source.rgb) * 10.f ? 1.f : 0.f;

    finalColor = source * halftone * 4.f;
}
