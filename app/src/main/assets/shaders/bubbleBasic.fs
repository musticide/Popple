#version 300 es
precision mediump float;

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;
in vec3 fragPosWS;

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
    vec3 camPos = vec3(0.0f, 20.0f, 0.0f);
    vec3 viewDir = normalize(camPos - fragPosWS);
    viewDir = normalize(-vec3(0.0, 1.0, 0.0));

    float fresnel = Fresnel(viewDir, fragNormal, 0.0f, 1.0f, 1.0f);

    vec3 R = reflect(-viewDir, fragNormal);

    vec3 envColor = texture(environmentMap, R).rgb;

    finalColor = fragColor * colDiffuse;
    // finalColor += vec4(envColor, 1.0);
    // finalColor.a = fresnel;
}
