#include "HUD.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "stb_image.h"

HUD::HUD(float sizeX, float sizeY, float posX, float posY, const char* textureNameIdle
	) : sizeX(sizeX), sizeY(sizeY), posX(posX), posY(posY)
{

	hudshader = new Shader("./res/shaders/basic2d.vert", "./res/shaders/basic2d.frag");

	float vertices[] = {
		posX, posY, 0.0f,  0.0f, 0.0f,
		 posX + sizeX, posY, 0.0f,  1.0f, 0.0f,
		 posX + sizeX,  posY + sizeY, 0.0f,  1.0f, 1.0f,
		 posX + sizeX,  posY + sizeY, 0.0f,  1.0f, 1.0f,
		posX,  posY + sizeY, 0.0f,  0.0f, 1.0f,
		posX, posY, 0.0f,  0.0f, 0.0f,
	};

	GLuint VBO;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &VBO);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenTextures(1, &activeTexture);
	glBindTexture(GL_TEXTURE_2D, activeTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	unsigned char* data = stbi_load(textureNameIdle, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);





}

HUD::HUD()
{
	hudshader = new Shader("./res/shaders/basic2d.vert", "./res/shaders/basic2d.frag");
}

void HUD::SetHud(float sizeX, float sizeY, float posX, float posY, const char* textureNameIdle) {

	float vertices[] = {
			posX, posY, 0.0f,  0.0f, 0.0f,
			 posX + sizeX, posY, 0.0f,  1.0f, 0.0f,
			 posX + sizeX,  posY + sizeY, 0.0f,  1.0f, 1.0f,
			 posX + sizeX,  posY + sizeY, 0.0f,  1.0f, 1.0f,
			posX,  posY + sizeY, 0.0f,  0.0f, 1.0f,
			posX, posY, 0.0f,  0.0f, 0.0f,
	};

	GLuint VBO;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &VBO);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenTextures(1, &activeTexture);
	glBindTexture(GL_TEXTURE_2D, activeTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	unsigned char* data = stbi_load(textureNameIdle, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);

}

void HUD::setLife(float Life)
{
	health = Life;
}

HUD::~HUD()
{
	GLuint activeTexture=NULL;
	GLuint textureIdle = NULL;
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(hudshader->ID);

}

void HUD::Draw()
{

	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	glDisable(GL_DEPTH_TEST);
	// Use our shader
	hudshader->use();

	
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, activeTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void HUD::Drawbar()
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	glDisable(GL_DEPTH_TEST);
	// Use our shader
	hudshader->use();
	float hp = health / 100.0f;
	hudshader->setFloat("LifeLevel", hp);
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, activeTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
