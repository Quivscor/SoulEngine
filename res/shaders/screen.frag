#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float contrast = 0.5;

void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;
    //col = (col - 0.5) * (1.0 + contrast) + 0.5;
    //col = vec3(1.0, col.y, 1.0) + 0.5;
    float bw = (col.x + col.y + col.z) / 3.0;
    FragColor = vec4(bw, bw, bw, 1.0);
} 