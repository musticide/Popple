#version 300 es
precision mediump float;

// Input vertex attributes (from vertex shader)
in vec4 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add your custom variables here
uniform vec4 _tfrc;

vec2 SpriteUV(vec2 uv, int frame, int rows, int columns) {
    int column = frame % columns;
    int row = frame / columns;

    vec2 spriteSize = vec2(1.0 / float(columns), 1.0 / float(rows));

    return uv * spriteSize +
        vec2(float(column), float(row)) * spriteSize;
}

vec2 SpriteUV(vec2 uv, float time, float fps, int rows, int columns, int indexOffset)
{
    int frame = int(time * fps) + indexOffset;
    int totalFrames = rows * columns;
    frame %= totalFrames;
    return SpriteUV(uv, frame, rows, columns);
}
vec3 OverlayFast(vec3 base, vec3 blend)
{
    return mix(
        2.0 * base * blend,
        1.0 - 2.0 * (1.0 - base) * (1.0 - blend),
        step(0.5, base)
    );
}

void main()
{
    // Texel color fetching from texture sampler
    int particleIndex = int(fragTexCoord.z);
    vec2 spriteUV = SpriteUV(fragTexCoord.xy, _tfrc.x, _tfrc.y, int(_tfrc.z), int(_tfrc.w), particleIndex);
    vec4 texelColor = texture(texture0, spriteUV);
    texelColor.rgb *= texelColor.a;

    finalColor = texelColor * colDiffuse * fragColor;
    finalColor.a = 0.0f;
}
