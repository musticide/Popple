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

void main()
{
    float ratio = _ScreenSize.x / _ScreenSize.y;
    vec2 uv = fragTexCoord;
    uv -= .5f;
    uv.x *= ratio;
    uv += .5f;

    vec4 source = texture(texture0, uv);
    finalColor = source;
}
