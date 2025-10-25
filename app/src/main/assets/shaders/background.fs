#version 300 es
precision mediump float;

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
// in vec3 fragNormal;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec4 colDiffuse2;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec2 uv = fragTexCoord;

    vec4 texelColor = texture(texture0, fragTexCoord);

    float glow = length((uv - .5f) * 2.f) - 0.3f;
    glow = glow * -1.f;
    glow = max(glow, 0.f);
    // uv = fract(uv * 20.f);
    // uv = (uv - .5f) * 2.f;
    // float checker = ceil(uv.x) * ceil(uv.y);
    // checker += floor(uv.x) * floor(uv.y);
    // checker = dot(uv.x, uv.y);

    // colDiffuse = vec3(.1f);
    // colDiffuse2 = vec3(.4f);

    // vec3 col = vec3(mix(.1f, .3f, checker));

    // finalColor = fragColor * vec4(col, 1);
    vec4 blue = vec4(.27f, .74f, .9f, 1.f);
    finalColor = texelColor + glow * blue;
}
