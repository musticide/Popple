#version 300 es
precision mediump float;

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec2 _ScreenSize;

// Output fragment color
out vec4 finalColor;

// const vec2 size = vec2(800, 450);   // Framebuffer size
const float samples = 5.0; // Pixels per axis; higher = bigger glow, worse performance
const float quality = 2.5; // Defines size factor: Lower = smaller glow, better quality

void main()
{
    vec2 size = _ScreenSize;
    vec4 sum = vec4(0);
    vec2 sizeFactor = vec2(1) / size * quality;

    // Texel color fetching from texture sampler
    vec4 source = texture(texture0, fragTexCoord);
    finalColor = source;

    const int range = 2; // should be = (samples - 1)/2;

    // for (int x = -range; x <= range; x++)
    // {
    //     for (int y = -range; y <= range; y++)
    //     {
    //         sum += texture(texture0, fragTexCoord + vec2(x, y) * sizeFactor);
    //     }
    // }

    // Calculate final fragment color
    // finalColor = ((sum / (samples * samples)) + source) * colDiffuse;
}
