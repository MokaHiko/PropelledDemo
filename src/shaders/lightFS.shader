#version 330 core
in vec2 TexCoords;
in vec4 LightColor;
out vec4 color;

void main()
{
    float distance = length(vec2(TexCoords.x - 0.5, TexCoords.y - 0.5));
    color = vec4(LightColor.rgb, 1 * (1 - distance));
}

