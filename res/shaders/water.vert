#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 aIndicators;

out vec3 Color;
out vec2 TexCoords;
out vec3 pass_normal;

const float PI = 3.1415926535897932384626433832795;

const float waveLength = 10.0;
//const float waveAmplitude = 1.0;

uniform mat4 transform;

uniform vec3 color;
uniform float waveTime;

vec3 calcNormal(vec3 vertex0, vec3 vertex1, vec3 vertex2) 
{
	vec3 tangent = vertex1 - vertex0;
	vec3 bitangent = vertex2 - vertex0;
	return normalize(cross(tangent, bitangent));
}

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
    vec3 currentVertex = vec3(aPos.x, aPos.y, aPos.z);
    vec3 vertex1 = currentVertex + vec3(aIndicators.x, aPos.y, aIndicators.y);
    vec3 vertex2 = currentVertex + vec3(aIndicators.z, aPos.y, aIndicators.w);

    currentVertex = applyDistortion(currentVertex);
    vertex1 = applyDistortion(vertex1);
    vertex2 = applyDistortion(vertex2);

    pass_normal = calcNormal(currentVertex, vertex1, vertex2);

    gl_Position = transform * vec4(applyDistortion(aPos), 1.0);
    Color = color;
    TexCoords = aTexCoords;
}