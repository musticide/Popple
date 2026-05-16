#version 300 es
precision mediump float;

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

// Input uniform values
// uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform samplerCube environmentMap;
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
    vec3 lightDir = normalize(lightPos);
    float lambert = max(dot(lightDir, fragNormal), 0.0f);

    float fresnel = Fresnel(viewDir, fragNormal, 0.0f, 1.0f, 1.0f);

    vec3 R = reflect(-viewDir, fragNormal);

    // vec4 texelColor = texture(texture0, fragTexCoord);

    vec3 envColor = texture(environmentMap, R).rgb;

    finalColor = mix(blue * .8f, blue, lambert);
    finalColor += vec4(envColor, 1.0) * fresnel;
}
