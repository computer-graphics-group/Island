#include "Floor.h"

Floor::Floor(unsigned int woodTexture){
	this->woodTexture = woodTexture;
	aoTexture = loadTexture(FileSystem::getPath("resources/textures/ao.png").c_str());

	glGenVertexArrays(1, &planeVAO1);
	glGenBuffers(1, &planeVBO1);
	glBindVertexArray(planeVAO1);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices1), planeVertices1, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);

	glGenVertexArrays(1, &planeVAO2);
	glGenBuffers(1, &planeVBO2);
	glBindVertexArray(planeVAO2);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices2), planeVertices2, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
}


Floor::~Floor()
{
	glDeleteVertexArrays(1, &planeVAO1);
	glDeleteBuffers(1, &planeVBO1);
	glDeleteVertexArrays(1, &planeVAO2);
	glDeleteBuffers(1, &planeVBO2);
}

void Floor::renderFloor(Shader& _shader) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	// floor
	glm::mat4 model = glm::mat4(1.0f);
	_shader.setMat4("model", model);
	glBindVertexArray(planeVAO1);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, aoTexture);
	glBindVertexArray(planeVAO2);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Floor::setDepthMap(unsigned int _depthMap) {
	depthMap = _depthMap;
}


