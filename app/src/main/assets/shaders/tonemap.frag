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

// sRGB => XYZ => D65_2_D60 => AP1 => RRT_SAT
const mat3 ACESInputMat = mat3(
        0.59719, 0.07600, 0.02840, // column 0
        0.35458, 0.90834, 0.13383, // column 1
        0.04823, 0.01566, 0.83777 // column 2
    );

const mat3 ACESOutputMat = mat3(
        1.60475, -0.10208, -0.00327, // column 0
        -0.53108, 1.10813, -0.07276, // column 1
        -0.07367, -0.00605, 1.07602 // column 2
    );

vec3 RRTAndODTFit(vec3 v)
{
    vec3 a = v * (v + 0.0245786f) - 0.000090537f;
    vec3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}

vec3 ACESFitted(vec3 color)
{
    color = ACESInputMat * color;

    // Apply RRT and ODT
    color = RRTAndODTFit(color);

    color = ACESOutputMat * color;

    // Clamp to [0, 1]
    color = clamp(color, 0.0, 1.0);

    return color;
}

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
    // source.rgb = pow(source.rgb, vec3(2.2f));
    // source.rgb *= 1.5f;
    source.rgb = ACESFitted(source.rgb);
    source.rgb = pow(source.rgb, vec3(.4545454545454545));

    finalColor = source;
}
