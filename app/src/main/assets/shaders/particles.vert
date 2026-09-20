#version 300 es
precision mediump float;

// Input vertex attributes
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;
layout(location = 5) in vec2 vertexTexCoord2;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;

// Output vertex attributes (to fragment shader)
out vec4 fragTexCoord;
out vec4 fragColor;

void main()
{
    fragTexCoord = vec4(vertexTexCoord.x, vertexTexCoord.y, vertexTexCoord2.x, vertexTexCoord2.y);
    fragColor = vertexColor;

    // Calculate final vertex position
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
