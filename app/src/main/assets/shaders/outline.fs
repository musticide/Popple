#version 300 es
precision mediump float;

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform float _OutlineSize;

// Output fragment color
out vec4 finalColor;

void main()
{
    // Texel color fetching from texture sampler
    vec2 texelSize = vec2(textureSize(texture0, 0));
    vec2 offset = vec2(1.0, -1.0) * (1.0 / texelSize) * 3.0;
    vec2 ratio = vec2(1.0, texelSize.x / texelSize.y);

    vec4 source = texture(texture0, fragTexCoord);
    vec4 source1 = abs(texture(texture0, fragTexCoord + offset.xx * ratio) - source);
    vec4 source2 = abs(texture(texture0, fragTexCoord + offset.xy * ratio) - source);
    vec4 source3 = abs(texture(texture0, fragTexCoord + offset.yx * ratio) - source);
    vec4 source4 = abs(texture(texture0, fragTexCoord + offset.yy * ratio) - source);
    vec4 sum = source1 + source2 + source3 + source4;

    float edge = clamp(sum.r + sum.g + sum.b, 0.f, 1.f);

    // finalColor = source * edge;
    finalColor = vec4(.15f, .22f, .25f, edge);
    // finalColor = vec4(.0f, .0f, .0f, edge);
}
