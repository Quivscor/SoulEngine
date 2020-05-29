#include "Billboard.h"

Billboard::Billboard()
{
	shaderbil = new Shader("./res/shaders/Billboard.vert", "./res/shaders/Billboard.frag");

	// Vertex shader
	 CameraRight_worldspace_ID = glGetUniformLocation(shaderbil->ID, "CameraRight_worldspace");
	 CameraUp_worldspace_ID = glGetUniformLocation(shaderbil->ID, "CameraUp_worldspace");
	 ViewProjMatrixID = glGetUniformLocation(shaderbil->ID, "VP");
	 BillboardPosID = glGetUniformLocation(shaderbil->ID, "BillboardPos");
	 BillboardSizeID = glGetUniformLocation(shaderbil->ID, "BillboardSize");
	 LifeLevelID = glGetUniformLocation(shaderbil->ID, "LifeLevel");

	 TextureID = glGetUniformLocation(shaderbil->ID, "myTextureSampler");


	 Texture = loadDDS("./res/textures/ExampleBillboard.DDS");

	// The VBO containing the 4 vertices of the particles.

}

Billboard::~Billboard()
{
}

void Billboard::drawbill(std::shared_ptr<Entity> cam)
{
	static const GLfloat g_vertex_buffer_data[] = {
	 -0.5f, -0.5f, 0.0f,
	  0.5f, -0.5f, 0.0f,
	 -0.5f,  0.5f, 0.0f,
	  0.5f,  0.5f, 0.0f,
	};
	GLuint billboard_vertex_buffer;
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);
	glm::mat4 ViewMatrix = glm::lookAt(
			cam->GetComponent<Transform>()->GetGlobalPosition(),           // Camera is here
		cam->GetComponent<Transform>()->GetGlobalPosition() + cam->GetComponent<Camera>()->lookVector, // and looks here : at the same position, plus "direction"
		cam->GetComponent<Camera>()->upVector                // Head is up (set to 0,-1,0 to look upside-down)
		);

	glm::vec3 CameraPosition(glm::inverse(ViewMatrix)[3]);
	// Use our shader
	glUseProgram(shaderbil->ID);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);

	// This is the only interesting part of the tutorial.
	// This is equivalent to mlutiplying (1,0,0) and (0,1,0) by inverse(ViewMatrix).
	// ViewMatrix is orthogonal (it was made this way), 
	// so its inverse is also its transpose, 
	// and transposing a matrix is "free" (inversing is slooow)
	glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glUniform3f(CameraUp_worldspace_ID, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

	glUniform3f(BillboardPosID, 0.0f, 0.5f, 0.0f); // The billboard will be just above the cube
	glUniform2f(BillboardSizeID, 1.0f, 0.125f);     // and 1m*12cm, because it matches its 256*32 resolution =)

	// Generate some fake life level and send it to glsl
	float LifeLevel =  0.1f + 0.7f;
	glUniform1f(LifeLevelID, LifeLevel);
	glm::mat4 ViewProjectionMatrix= cam->GetComponent<Camera>()->GetProjection()* cam->GetComponent<Transform>()->GetGlobalMatrix();
	glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	// 1rst attribute buffer : vertices
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


	// Draw the billboard !
	// This draws a triangle_strip which looks like a quad.
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

}

GLuint Billboard::loadDDS(std::string path)
{
	unsigned char header[124];

	FILE* fp;
	char cstr[124];
	strcpy(cstr, path.c_str());
	/* try to open the file */
	fp = fopen(cstr, "rb");
	if (fp == NULL) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", cstr); getchar();
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
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