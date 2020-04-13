#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Color;
out vec2 TexCoords;

uniform mat4 transform;
uniform vec3 color;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
    Color = color;
	TexCoords = aTexCoords;
}