#version 300 es
precision mediump float;

// Input vertex attributes
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;
uniform mat4 matModel;

// Output vertex attributes (to fragment shader)
out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragNormal;
out vec3 fragPosWS;

void main()
{
    // Send vertex attributes to fragment shader
    fragTexCoord = vertexTexCoord;

    fragColor = vertexColor;
    fragNormal = vertexNormal;
    fragPosWS = vec3(matModel * vec4(vertexPosition, 1.0));

    // Calculate final vertex position
    vec4 posCS = mvp * vec4(vertexPosition, 1.0);
    gl_Position = posCS;
}
