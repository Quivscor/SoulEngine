#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
// Interpolated values from the vertex shaders

// texture samplers
uniform sampler2D texture1;
uniform float LifeLevel;
void main()
{
	//FragColor = texture(texture1, TexCoord);
	FragColor = texture( texture1, TexCoord );
	
	// Hardcoded life level, should be in a separate texture.
	if(LifeLevel>0)
	{
	if (TexCoord.x <LifeLevel && TexCoord.y > 0.3 && TexCoord.y < 0.7 && TexCoord.x > 0.01 )
	{
	if(LifeLevel<0.3)
	FragColor = vec4(0.9, 0.1, 0.2, 1.0);
	else
		FragColor = vec4(0.2, 0.8, 0.2, 1.0); // Opaque green
	}
	}
	
}