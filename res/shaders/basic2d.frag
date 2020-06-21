#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
// Interpolated values from the vertex shaders

// texture samplers
uniform sampler2D texture1;
uniform float LifeLevel;
uniform vec4 color;
uniform bool horizontal;
uniform bool fill;
uniform bool textureColor;
void main()
{
	//FragColor = texture(texture1, TexCoord);
	FragColor = texture( texture1, TexCoord );
	if(fill == true)
	{
		if(horizontal == true)
		{
			if(color.r == 0 && color.g == 0 &&color.b == 0)
			{	// Hardcoded life level, should be in a separate texture.
				if(LifeLevel > 0)
				{
					if (TexCoord.x <LifeLevel && TexCoord.y > 0.01 && TexCoord.y < 0.99 && TexCoord.x > 0.01 )
					{
						
						FragColor = vec4(0.9, 0.1, 0.2, 1.0);
					}
				}
			}
			else
			{
				if(LifeLevel > 0)
				{
					if (TexCoord.x <LifeLevel && TexCoord.y > 0.01 && TexCoord.y < 0.99 && TexCoord.x > 0.01 )
					{
						FragColor = color;
					}
				}
			}
		}
		else
		{
			if(color.r == 0 && color.g == 0 && color.b == 0)
			{	// Hardcoded life level, should be in a separate texture.
				if(LifeLevel > 0)
				{
					if (TexCoord.x < 0.98  && TexCoord.y > 0.02 && TexCoord.y < LifeLevel && TexCoord.x > 0.02)
					{
						FragColor = vec4(0.9, 0.1, 0.2, 1.0);
					}
				}
			}
			else
			{
				if(LifeLevel>0)
				{
					if (TexCoord.x <0.9  && TexCoord.y > 0.01 && TexCoord.y < LifeLevel&& TexCoord.x >0.1  )
					{
						FragColor = color;
					}
				}
			}
		}
	}
	if(textureColor == true)
	{
		FragColor = texture( texture1, TexCoord ) * color;
	}
}