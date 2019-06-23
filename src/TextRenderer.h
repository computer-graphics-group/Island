#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>

#include <iostream>
#include <sstream>
#include <ft2build.h>
#include <map>
#include FT_FREETYPE_H

class TextRenderer {

	
public:
	struct Character {
		GLuint TextureID;   // ID handle of the glyph texture
		glm::ivec2 Size;    // Size of glyph
		glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
		GLuint Advance;    // Horizontal offset to advance to next glyph
	};


	std::map<GLchar, Character> Characters;
	
	unsigned int VAO, VBO;
	TextRenderer();
	void init();
	void setSize(const unsigned int width, const unsigned int height);
	void RenderWoodNum(Shader &shader, int num);
	void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	
private:
	// settings
	unsigned int width = 1920;
	unsigned int height = 1000;
};
