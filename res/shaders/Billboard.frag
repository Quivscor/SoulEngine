#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

uniform sampler2D myTextureSampler;

uniform float LifeLevel;

void main(){
	// Output color = color of the texture at the specified UV
	color = texture( myTextureSampler, UV );
	
	// Hardcoded life level, should be in a separate texture.
	if (UV.x < LifeLevel - 0.05 && UV.y > 0.3 && UV.y < 0.7 && UV.x > 0.05 )
	{
		color = vec4(0.9, 0.1, 0.2, 1.0);
	}
	
}