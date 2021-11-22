#version 330 core
layout(location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
out vec4 LightColor;

uniform mat4 projection;
uniform vec4 color;
uniform vec2 offset;

void main()
{
    TexCoords = vertex.zw;
    LightColor = color;
    gl_Position = projection * vec4((vertex.xy * 200) + offset, 0.0, 1.0);
}
