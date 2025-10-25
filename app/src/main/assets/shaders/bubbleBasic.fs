#version 300 es
precision mediump float;

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

// Input uniform values
// uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

float Fresnel(vec3 viewDir, vec3 normal, float bias, float scale, float power)
{
    float fresnel = scale * pow(1.0 + dot(viewDir, normal), power);
    return clamp(fresnel, 0.0, 1.0);
}

void main()
{
    vec3 viewDir = normalize(-vec3(0.0, 1.0, 0.0));
    float fresnel = Fresnel(viewDir, fragNormal, 0.0f, 1.0f, 1.0f);
    // Texel color fetching from texture sampler
    // vec4 texelColor = texture(texture0, fragTexCoord);

    finalColor = fragColor * colDiffuse;
    finalColor.a = fresnel;
}
