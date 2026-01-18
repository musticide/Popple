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

vec3 ACESFilm(vec3 x)
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main()
{
    vec4 source = texture(texture0, fragTexCoord);
    //ACES Tonemap
    source.rgb *= 0.8f;
    source.rgb = ACESFilm(source.rgb);

    source.rgb = pow(source.rgb, vec3(.454545454545));

    finalColor = source;
}
