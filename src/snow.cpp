#include "snow.h"

ParticleSystem::ParticleSystem(int number, glm::vec3 genPosition, float range_x, float range_z, float skyHeight) {
	this->number = number;
	this->GenPos = genPosition;
	this->range_x = range_x;
	this->range_z = range_z;
	this->skyHeight = skyHeight;

	initParticleSystem();

}

void ParticleSystem::initParticleSystem() {
	particles.resize(number);
	
	GLuint VBO;	
	GLfloat particlev[] = {
		0.0f, 0.25f, 0.0f, 0.25f,
		0.25f, 0.0f, 0.25f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 0.25f, 0.0f, 0.25f,
		0.25f, 0.25f, 0.25f, 0.25f,
		0.25f, 0.0f, 0.25f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particlev), particlev, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(float)));
	glBindVertexArray(0);

	float x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, dec, rx, ry, rv;
	for (Particle &particle : particles) {
		//位置
		x = range_x * (rand() / float(RAND_MAX)) - range_x / 2;
		z = range_z * (rand() / float(RAND_MAX)) - range_z / 2;
		y = skyHeight;
		particle.position = glm::vec3(x, y, z);
		//大小
		particle.scale = rand() / float(RAND_MAX) * (0.07f - 0.05f) + 0.05f;
		//速度
		vx = rand() / float(RAND_MAX) *  0.05f;
		vy = rand() / float(RAND_MAX) * (0.1f - 0.05f) + 0.05f;
		vz = rand() / float(RAND_MAX) *  0.1f;
		particle.velocity = glm::vec3(vx, vy, vz);
		//角度
		rx = (rand() / float(RAND_MAX)) * 360.0f;
		ry = (rand() / float(RAND_MAX))  * 360.0f;
		//旋转
		rv = rand() / float(RAND_MAX) * 0.1f;
		particle.rotateAngle = glm::vec2(rx, ry);
		particle.rotateSpeed = rand() / float(RAND_MAX) * 0.1f;
		ax = 0;
		ay = 100.0f;
		az = 0;
		particle.acceleration = glm::vec3(ax, ay, az);
		particle.lifetime = 100.0f;
		//消失
		dec = rand() / float(RAND_MAX) * 0.05f;
		particle.dec = dec;
		
	}

}

void ParticleSystem::Update(float dt, glm::vec3 pos) {

	for (Particle &particle : particles) {
		position = particle.position;
		velocity = particle.velocity;
		acceleration = particle.acceleration;
		lifetime = particle.lifetime;
		dec = particle.dec;
		position.x += velocity.x;
		position.y -= velocity.y;

		velocity.y += acceleration.y * dt * 0.000005f;
		lifetime -= dec * dt * 100.0f;
		if (position.y <= -1 || lifetime <= 0)
		{
			
			float x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, dec, rx, ry, rv;
			//位置
			x = range_x * (rand() / float(RAND_MAX)) - range_x / 2;
			z = range_z * (rand() / float(RAND_MAX)) - range_z / 2;
			y = skyHeight;
			particle.position = glm::vec3(x, y, z);
			//大小
			particle.scale = rand() / float(RAND_MAX) * (0.07f - 0.05f) + 0.05f;
			//速度
			vx = rand() / float(RAND_MAX) *  0.05f;
			vy = rand() / float(RAND_MAX) * (0.1f - 0.05f) + 0.05f;
			vz = rand() / float(RAND_MAX) *  0.1f;
			particle.velocity = glm::vec3(vx, vy, vz);
			//角度
			rx = (rand() / float(RAND_MAX)) * 360.0f;
			ry = (rand() / float(RAND_MAX))  * 360.0f;
			//旋转
			rv = rand() / float(RAND_MAX) * 0.1f;
			particle.rotateAngle = glm::vec2(rx, ry);
			particle.rotateSpeed = rand() / float(RAND_MAX) * 0.1f;
			ax = 0;
			ay = 100.0f;
			az = 0;
			particle.acceleration = glm::vec3(ax, ay, az);
			particle.lifetime = 100.0f;
			//消失
			dec = rand() / float(RAND_MAX) * 0.05f;
			particle.dec = dec;
			return;
		}

		particle.position = position;
		particle.velocity = velocity;
		particle.acceleration = acceleration;
		particle.lifetime = lifetime;
		particle.dec = dec;

	}
	//pos == camera.postion
	GenPos = pos;
}

void ParticleSystem::Draw(Shader& shader) {
	shader.use();
	glBindVertexArray(VAO);
	//unsigned int snow = loadTexture(FileSystem::getPath("resources/textures/snow.png").c_str());//"resources/textures/snow.png"
	//unsigned int snowTexture = loadTexture(FileSystem::getPath("resources/textures/snow.png").c_str());//"resources/textures/snow.png"
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, snowTexture);
	//shader.setInt("snow", snowTexture);
	for (Particle &particle : particles) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, particle.position);
		model = glm::translate(model, GenPos);
		model = glm::rotate(model, glm::radians(particle.rotateAngle.x * particle.rotateSpeed * (float)glfwGetTime()), glm::vec3(1.0, 0.0, 0.0));
		model = glm::rotate(model, glm::radians(particle.rotateAngle.y * particle.rotateSpeed * (float)glfwGetTime()), glm::vec3(0.0, 1.0, 0.0));
		shader.setMat4("model", model);
		shader.setFloat("scale", particle.scale);
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}
	glBindVertexArray(0);

}
