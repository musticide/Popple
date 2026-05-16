#version 300 es
precision mediump float;

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform float _Threshold;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec4 source = texture(texture0, fragTexCoord);
    source.rgb -= _Threshold;
    source.rgb = max(source.rgb, vec3(0.f));
    finalColor = source;
}
