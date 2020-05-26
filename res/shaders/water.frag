#version 330 core
out vec4 FragColor;

in vec3 Color;
in vec2 TexCoords;
in vec3 pass_normal;

uniform sampler2D texture_diffuse1;
uniform bool hasTexture;

void main()
{
	if (hasTexture)
		FragColor = texture(texture_diffuse1, TexCoords) * vec4(Color, 1.0f);
	else
		FragColor = vec4((vec3(1.0, 1.0, 1.0) - pass_normal) * Color, 1.0f);
}