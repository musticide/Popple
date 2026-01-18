#version 300 es
precision mediump float;

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;

// Input uniform values
uniform sampler2D texture0;
uniform vec2 _Direction;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec2 texelSize = 1.f / vec2(textureSize(texture0, 0));

    float offsets[] = float[](
            -4.0,
            -3.0,
            -2.0,
            -1.0,
            0.0,
            1.0,
            2.0,
            3.0,
            4.0
        );
    float weights[] = float[](
            0.01621622,
            0.05405405,
            0.12162162,
            0.19459459,
            0.22702703,
            0.19459459,
            0.12162162,
            0.05405405,
            0.01621622
        );

    vec3 color = vec3(0.f);

    for (int i = 0; i < 9; i++) {
        // float offset = offsets[i] * 2.0 * GetSourceTexelSize().x;
        // color += GetSource(fragTexCoord + (offset, 0.0)).rgb * weights[i];
        vec2 offset = offsets[i] * 2.f * texelSize * _Direction;
        color += texture(texture0, fragTexCoord + offset).rgb * weights[i];
    }

    finalColor = vec4(color, 1.f);
}
