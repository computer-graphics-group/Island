#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <vector>
class WaveSea {
public:
	WaveSea();
	void initGerstnerWave();
	void Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 position);
	void initShader();

private:
	const float PI = 3.14159265359;
	Shader shader = Shader("ShaderProgram/sea.vs", "ShaderProgram/sea.fs");

	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;

	unsigned int VAO, VBO, EBO;
	unsigned int indexCount;

	std::vector<unsigned int> indices;

	int waveCount = 2;
	double waveLength[4] = { 0.2, 0.6, 1.2, 2.1 };
	double speed[4] = { 0.2,0.5,0.05, 0.02 };

	glm::vec2 direction[4] = {
		glm::vec2(1.0, 0.0),
		glm::vec2(1.0, 1.0),
		glm::vec2(0.5, 0.9),
		glm::vec2(1.0, 1.0),
	};

	double amplitude[4] = { 0.02, 0.02, 0.02, 0.01 };
	double steepness[4]{ 3.0, 1.0, 1.5, 3.0 };
	// set color
	glm::vec3 seaColor = glm::vec3(0.33, 0.45, 0.57);
};