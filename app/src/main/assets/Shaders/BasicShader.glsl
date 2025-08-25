#shader vertex
#version 300 es
precision mediump float;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoord;

out vec2 texCoord;
void main()
{
    texCoord = a_texCoord;
    gl_Position = vec4(a_position, 1.0);
}

#shader fragment
#version 300 es
precision mediump float;
in vec2 texCoord;
out vec4 FragColor;
void main()
{
    // gl_fragcolor = vec4(1.0, 0.0, 0.0, 1.0);
    FragColor = vec4(texCoord.x, texCoord.y, 0.0, 1.0);
}
