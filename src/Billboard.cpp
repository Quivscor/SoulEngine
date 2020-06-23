
#include "Billboard.h"



Billboard::Billboard(char* imagepath, bool x)
{
	shaderbil = new Shader("./res/shaders/Billboard.vert", "./res/shaders/Billboard.frag");

	static const GLfloat g_vertex_buffer_data[] = {
 -0.5f, -0.5f, 0.0f,
  0.5f, -0.5f, 0.0f,
 -0.5f,  0.5f, 0.0f,
  0.5f,  0.5f, 0.0f,
	};
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


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
	if (x)
	{

		Texture = loadDDS(imagepath);
	}
	else
	{
		glGenTextures(1, &Texture);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		//stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(imagepath, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(data);
	}
	// char* imagepath = "./res/textures/stone.jpg";
	//
	
}
Billboard::~Billboard()
{
	Texture = NULL;
	glDeleteBuffers(1, &billboard_vertex_buffer);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(shaderbil->ID);
	
}
Billboard::Billboard()
{
shaderbil = new Shader("./res/shaders/Billboard.vert", "./res/shaders/Billboard.frag");

//	this->type;


this->LifeLevel = 1.0f;
Texture=NULL;
billboard_vertex_buffer=NULL;
vao = NULL;
}

//const char* imagepath = "./res/textures/ExampleBillboard.DDS";

void Billboard::setLife(float life)
{
	std::cout << life<< std::endl;
	this->LifeLevel = life/1.0f;
	
}
void Billboard::Draw( std::shared_ptr<Entity>  camera, glm::vec3 position, glm::vec2 size )
{

	glDisable(GL_DEPTH_TEST);	glDepthFunc(GL_ALWAYS);
	//std::cout << this->LifeLevel << std::endl;
	int w = 1920, h = 1080; //fix later
	//	glm::vec2 size = glm::vec2(1.f, 0.125f);

	// Vertex shader
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.01f, 10000.0f);
	///glm::mat4 text_matrix_2D = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f);

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
	glBindTexture(GL_TEXTURE_2D,Texture);

	// This is the only interesting part of the tutorial.
		// This is equivalent to mlutiplying (1,0,0) and (0,1,0) by inverse(ViewMatrix).
		// ViewMatrix is orthogonal (it was made this way), 
		// so its inverse is also its transpose, 
		// and transposing a matrix is "free" (inversing is slooow)
	glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glUniform3f(CameraUp_worldspace_ID, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

	glUniform3f(BillboardPosID, position.x, position.y, position.z); // The billboard will be just above the cube
	glUniform2f(BillboardSizeID, size.x, size.y);     // and 1m*12cm, because it matches its 256*32 resolution =)

	//this->LifeLevel = 0.2f;

	glUniform1f(LifeLevelID, this->LifeLevel);
	glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDepthFunc(GL_LESS);

	//delete this; //freed memory
	
}
void Billboard::SetBillboard(char* imagepath, bool x)
{
	
	
	static const GLfloat g_vertex_buffer_data[] = {
-0.5f, -0.5f, 0.0f,
 0.5f, -0.5f, 0.0f,
-0.5f,  0.5f, 0.0f,
 0.5f,  0.5f, 0.0f,
	};
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


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
	if (x)
	{

		Texture = loadDDS(imagepath);
	}
	else
	{
		glGenTextures(1, &Texture);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;

		unsigned char* data = stbi_load(imagepath, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(data);
	}
}
GLuint Billboard::loadDDS(const char* imagepath)
{
	unsigned char header[124];
	
	FILE* fp;
	char cstr[124];
	
	/* try to open the file */
	fp = fopen(imagepath, "rb");
	
	if (fp == NULL) {
		std::cout << cstr << std::endl;
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		std::cout << cstr << std::endl;
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);


	unsigned char* buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if (width < 1) width = 1;
		if (height < 1) height = 1;

	}

	free(buffer);

	return textureID;
}