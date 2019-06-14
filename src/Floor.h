#pragma once
#include <learnopengl/shader.h>
#include <learnopengl/filesystem.h>
#include <stb_image.h>
class Floor
{
public:
	Floor(unsigned int texture1);
	~Floor();
	void renderFloor(Shader& _shader);
	void setDepthMap(unsigned int depthMap);

private:
	// meshes
	unsigned int planeVAO1, planeVAO2;
	// plane VAO
	unsigned int planeVBO1, planeVBO2;
	unsigned int woodTexture, aoTexture, depthMap;
	float planeVertices1[48] = {
		// positions            // normals         // texcoords
		 0.0f, -0.5f,  100.0f,  0.0f, 1.0f, 0.0f,  100.0f,  0.0f,
		-100.0f, -0.5f,  100.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-100.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,   0.0f, 100.0f,

		 0.0f, -0.5f,  100.0f,  0.0f, 1.0f, 0.0f,  100.0f,  0.0f,
		-100.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,   0.0f, 100.0f,
		 0.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,  100.0f, 100.0f
	};
	float planeVertices2[48] = {
		// positions            // normals         // texcoords
		 0.0f, -0.5f,  100.0f,  0.0f, 1.0f, 0.0f,  100.0f,  0.0f,
		 100.0f, -0.5f,  100.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		 100.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,   0.0f, 100.0f,

		 0.0f, -0.5f,  100.0f,  0.0f, 1.0f, 0.0f,  100.0f,  0.0f,
		 0.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,   0.0f, 100.0f,
		 100.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,  100.0f, 100.0f
	};
};
// utility function for loading a 2D texture from file
// ---------------------------------------------------
static unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
