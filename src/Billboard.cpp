
#include "Billboard.h"
#include "stb_image.h"


Billboard::Billboard()
{
	const char* textureName = "ExampleBillboard.DDS";
	static const GLfloat g_vertex_buffer_data[] = {
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f,
	};
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint billboard_vertex_buffer;
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	unsigned char* data = stbi_load(textureName, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);
}

Billboard::~Billboard()
{
}

void Billboard::Draw( std::shared_ptr<Entity>  camera, glm::vec3 position)
{
	glDepthFunc(GL_ALWAYS);

	int w = 1280, h = 720; //fix later
	glm::vec2 size = glm::vec2(2.0f, 0.125f);
	shaderbil = new Shader("./res/shaders/Billboard.vert", "./res/shaders/Billboard.frag");
	
		// Vertex shader
		 glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.01f, 10000.0f);

	glm::vec3 cameraUp = camera->GetComponent<Camera>()->upVector;
	glm::vec3 cameraRight = camera->GetComponent<Transform>()->Right();
	glm::vec3 cameraPos = camera->GetComponent<Transform>()->GetGlobalPosition();
	glm::mat4 ViewMatrix = camera->GetComponent<Transform>()->GetGlobalMatrix();
	glm::mat4 ViewProjectionMatrix = Projection * ViewMatrix;

	GLuint CameraRight_worldspace_ID = glGetUniformLocation(shaderbil->ID, "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID = glGetUniformLocation(shaderbil->ID, "CameraUp_worldspace");
	GLuint ViewProjMatrixID = glGetUniformLocation(shaderbil->ID, "VP");
	GLuint BillboardPosID = glGetUniformLocation(shaderbil->ID, "BillboardPos");
	GLuint BillboardSizeID = glGetUniformLocation(shaderbil->ID, "BillboardSize");
	GLuint LifeLevelID = glGetUniformLocation(shaderbil->ID, "LifeLevel");
	// Use our shader
	shaderbil->use();
	glBindVertexArray(vao);

	//// Bind our texture in Texture Unit 0w
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	// This is the only interesting part of the tutorial.
		// This is equivalent to mlutiplying (1,0,0) and (0,1,0) by inverse(ViewMatrix).
		// ViewMatrix is orthogonal (it was made this way), 
		// so its inverse is also its transpose, 
		// and transposing a matrix is "free" (inversing is slooow)
	glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glUniform3f(CameraUp_worldspace_ID, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

	glUniform3f(BillboardPosID, position.x, position.y, position.z); // The billboard will be just above the cube
	glUniform2f(BillboardSizeID, size.x, size.y);     // and 1m*12cm, because it matches its 256*32 resolution =)
	float LifeLevel = sin(TimeCustom::GetTime()) * 0.1f + 0.7f;
	glUniform1f(LifeLevelID, LifeLevel);
	glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDepthFunc(GL_LESS);
}