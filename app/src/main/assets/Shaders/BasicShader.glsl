#shader vertex
#version 300 es
precision mediump float;

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;

uniform mat4 u_MODEL;

out vec2 v_TexCoord;
void main()
{
    v_TexCoord = a_texCoord;
    gl_Position = u_MODEL * vec4(a_position, 1.0);
}

#shader fragment
#version 300 es
precision mediump float;

in vec2 v_TexCoord;

out vec4 FragColor;

uniform sampler2D u_Texture;

void main()
{
    // gl_fragcolor = vec4(1.0, 0.0, 0.0, 1.0);
    // FragColor = vec4(v_TexCoord.x, v_TexCoord.y, 0.0, 1.0);
    FragColor = texture(u_Texture, v_TexCoord);
}
