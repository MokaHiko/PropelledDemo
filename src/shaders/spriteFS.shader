#version 330 core
in vec2 TexCoords;
in vec2 FragPos;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform float lightDistance;

void main()
{
    float ambient = 1;
	color = vec4(1.0, 1.0, 1.0, 1.0);
	color = vec4(spriteColor * ambient * lightDistance, (1)) * texture(image, TexCoords);
}