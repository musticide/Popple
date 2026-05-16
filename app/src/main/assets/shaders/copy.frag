#version 300 es
precision mediump float;

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;

// Output fragment color
out vec4 finalColor;

void main()
{
    // Texel color fetching from texture sampler
    vec4 source = texture(texture0, fragTexCoord);
    finalColor = source;
}
