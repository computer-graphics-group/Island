#include "WaveSea.h"

WaveSea::WaveSea() {
	initGerstnerWave();
	initShader();

}

void WaveSea::initGerstnerWave() {

	bool oddRow = false;
	for (int y = 0; y < Y_SEGMENTS - 1; ++y)
	{
		if (!oddRow) // even rows: y == 0, y == 2; and so on
		{
			for (int x = 0; x < X_SEGMENTS; ++x)
			{
				indices.push_back( y      * (X_SEGMENTS)+x);
				indices.push_back((y + 1) * (X_SEGMENTS)+x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS - 1; x >= 0; --x)
			{
				indices.push_back((y + 1) * (X_SEGMENTS)+x);
				indices.push_back( y      * (X_SEGMENTS)+x);
			}
		}
		oddRow = !oddRow;
	}
	indexCount = indices.size();

	std::vector<float> pos_;
	for (unsigned int x = 0; x < X_SEGMENTS; ++x)
	{
		for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
		{

			float xPos = float((float)x / X_SEGMENTS);
			float yPos = float((float)y / Y_SEGMENTS);
			float zPos = 0.0f;

			pos_.push_back(xPos);
			pos_.push_back(yPos);
			pos_.push_back(zPos);
		}
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (X_SEGMENTS * Y_SEGMENTS * 3) * sizeof(float), &pos_[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);
}

void WaveSea::Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 position) {

	glBindVertexArray(VAO);

	shader.use();
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(x, y, z));
	//model = glm::scale(model, glm::vec3(x1, y1, z1));

	model = glm::translate(model, glm::vec3(-100.0f, -5.0f, -100.0f));
	model = glm::scale(model, glm::vec3(300.0f, 20.0f, 300.0f));

	shader.setMat4("model", model);
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setVec3("viewPos", position);

	float t = glfwGetTime();
	shader.setFloat("t", t);
	glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);


}


void WaveSea::initShader() {
	shader.use();
	for (int i = 0; i < waveCount; i++) {
		char str1[40], str2[40], str3[40], str4[40];
		sprintf(str1, "direction[%d]", i);
		shader.setVec2(str1, direction[i]);

		sprintf(str2, "amplitude[%d]", i);
		shader.setFloat(str2, amplitude[i]);

		sprintf(str3, "waveLength[%d]", i);
		shader.setFloat(str3, waveLength[i]);

		sprintf(str4, "speed[%d]", i);
		shader.setFloat(str4, speed[i]);
	}

	shader.setFloat("PI", PI);
	
	//set light
	glm::vec3 lightPos = glm::vec3(0.0f, 100.0f, 0.0f);
	glm::vec3 lightDirection = glm::vec3(-1.0, -1.0, -1.0);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightAmbient = glm::vec3(0.8f, 0.8f, 0.8f);
	glm::vec3 lightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 lightSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
	shader.setVec3("dirLight.direction", lightDirection);
	shader.setVec3("dirLight.color", lightColor);
	shader.setVec3("dirLight.ambient", lightAmbient);
	shader.setVec3("dirLight.diffuse", lightDiffuse);
	shader.setVec3("dirLight.specular", lightSpecular);

	//set color
	shader.setVec3("objectColor", seaColor);

}
