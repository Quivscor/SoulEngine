#pragma once
#include "Core.h"


#include <iostream>
#include <map>
#include <string>
// for text rendering
#include "ft2build.h"
#include FT_FREETYPE_H

#include "Shader.h"
#ifndef TextRendering_class
#define TextRendering_class




class TextRendering
{
	struct Character
{
	GLuint texture_id;  // glyph texture
	glm::ivec2 size; // size of glyph
	glm::vec2 bearing;  // offset from the baseline to left/top of glyph
	GLuint  advance;  // offset to advance to next glyph
};

public:
	static TextRendering* Instance()
	{
		static TextRendering inst;
		return &inst;
	}

	void draw(std::string text, glm::vec3 color, glm::mat4 matrix);

private:
	TextRendering();
	~TextRendering();

	GLuint VAO, VBO;
	Shader* shadert;

	std::map<GLchar, Character> characters;
};

#endif
