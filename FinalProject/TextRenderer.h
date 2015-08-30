#pragma once

// FreeType
#include <ft2build.h>
#include "Shader.h"
#include <iostream>
#include "Constants.h"
#include <map>
#include FT_FREETYPE_H

/// Holds all state information relevant to a character as loaded using FreeType
struct Character 
{
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

class TextRenderer
{
	std::map<GLchar, Character> Characters;
	GLuint VAO, VBO;
	Shader shader;

public:
	TextRenderer(Shader &shader);
	~TextRenderer();

	void initRenderData();
	void DrawText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};

