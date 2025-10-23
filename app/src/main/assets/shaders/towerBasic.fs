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
    vec4 blue = vec4(.27f, .74f, .9f, 1.f);
    vec3 viewDir = normalize(-vec3(0.0, 1.0, 0.0));
    vec3 lightPos = vec3(10, 10, -10);
    float lambert = max(dot(lightPos, fragNormal), 0.0f);

    // Texel color fetching from texture sampler
    // vec4 texelColor = texture(texture0, fragTexCoord);

    // finalColor = fragColor * colDiffuse;
    finalColor = mix(blue * .8f, blue, lambert);
    // finalColor = vec4(fragNormal, 1.0f);
}
