#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Color;
out vec2 TexCoords;

const float PI = 3.1415926535897932384626433832795;

const float waveLength = 10.0;
//const float waveAmplitude = 1.0;

uniform mat4 transform;

uniform vec3 color;
uniform float waveTime;

float generateOffset(float x, float z)
{
	float radiansX = (x / waveLength + waveTime) * 2.0 * PI;
	float radiansZ = (z / waveLength + waveTime) * 2.0 * PI;
	return 0.5 * (sin(radiansZ) + cos(radiansX));
}

vec3 applyDistortion(vec3 vertex) 
{
	float xDistortion = generateOffset(vertex.x, vertex.z);
	float yDistortion = generateOffset(vertex.x, vertex.z);
	float zDistortion = generateOffset(vertex.x, vertex.z);
	return vertex + vec3(xDistortion, yDistortion, zDistortion);
}

void main()
{
    gl_Position = transform * vec4(applyDistortion(aPos), 1.0);
    Color = color;
    TexCoords = aTexCoords;
}