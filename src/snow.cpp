#include "snow.h"
//#include"Floor.h"//LoadTexture

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
	//initParticleVAO();
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
	float r, g, b, a;
	for (Particle &particle : particles) {
		//初始化颜色（白色）
		r = 1.0f;
		g = 1.0f;
		b = 1.0f;
		a = 1.0f;
		particle.color = glm::vec4(r, g, b, a);

		//初始化坐标
		x = range_x * (rand() / float(RAND_MAX)) - range_x / 2;
		z = range_z * (rand() / float(RAND_MAX)) - range_z / 2;
		y = skyHeight;
		particle.position = glm::vec3(x, y, z);

		//初始化速度
		vx = rand() / float(RAND_MAX) *  0.05f;
		vy = rand() / float(RAND_MAX) * (0.1f - 0.05f) + 0.05f;
		vz = rand() / float(RAND_MAX) *  0.1f;
		particle.velocity = glm::vec3(vx, vy, vz);
		//旋转角度
		rx = (rand() / float(RAND_MAX)) * 360.0f;
		ry = (rand() / float(RAND_MAX))  * 360.0f;
		//旋转速度
		//rv = rand() / float(RAND_MAX) * 0.1f;

		particle.rotateAngle = glm::vec2(rx, ry);
		particle.rotateSpeed = rand() / float(RAND_MAX) * 0.1f;

		//初始化加速度
		ax = 0;
		ay = 100.0f;//G_ACCE;
		az = 0;
		particle.acceleration = glm::vec3(ax, ay, az);

		//初始化生命周期
		particle.lifetime = 100.0f;

		//消失速度
		dec = rand() / float(RAND_MAX) * 0.05f;
		particle.dec = dec;

		//初始化大小
		particle.scale = rand() / float(RAND_MAX) * (0.1f - 0.05f) + 0.05f;
		//respawnParticle(particle);
	}

}
/*
void ParticleSystem::initParticleVAO() {
	GLuint VBO;
	GLfloat particlev[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
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
}



void ParticleSystem::respawnParticle(Particle &particle) {
	//初始化颜色（白色）
	r = 1.0f;
	g = 1.0f;
	b = 1.0f;
	a = 1.0f;
	particle.color = glm::vec4(r, g, b, a);

	//初始化坐标
	x = range_x * (rand() / float(RAND_MAX)) - range_x / 2;
	z = range_z * (rand() / float(RAND_MAX)) - range_z / 2;
	y = skyHeight;
	particle.position = glm::vec3(x, y, z);

	//初始化速度
	vx = rand() / float(RAND_MAX) *  0.05f;
	vy = rand() / float(RAND_MAX) * (0.1f - 0.05f) + 0.05f;
	vz = rand() / float(RAND_MAX) *  0.1f;
	particle.velocity = glm::vec3(vx, vy, vz);
	//旋转角度
	rx = (rand() / float(RAND_MAX)) * 360.0f;
	ry = (rand() / float(RAND_MAX))  * 360.0f;
	//旋转速度
	//rv = rand() / float(RAND_MAX) * 0.1f;

	particle.rotateAngle = glm::vec2(rx, ry);
	particle.rotateSpeed = rand() / float(RAND_MAX) * 0.1f;
	
	//初始化加速度
	ax = 0;
	ay = 100.0f;//G_ACCE;
	az = 0;
	particle.acceleration = glm::vec3(ax, ay, az);

	//初始化生命周期
	particle.lifetime = 100.0f;

	//消失速度
	dec = rand() / float(RAND_MAX) * 0.05f;
	particle.dec = dec;

	//初始化大小
	particle.scale = rand() / float(RAND_MAX) * (0.1f - 0.05f) + 0.05f;

}

*/
void ParticleSystem::Update(float dt, glm::vec3 pos) {

	for (Particle &particle : particles) {
		/** 更新位置 */
		position = particle.position;
		velocity = particle.velocity;
		acceleration = particle.acceleration;
		lifetime = particle.lifetime;
		dec = particle.dec;

		position.x += velocity.x;
		position.y -= velocity.y;

		/** 更新速度 */
		velocity.y += acceleration.y * dt * 0.000005f;

		/** 更新生存时间 */
		lifetime -= dec * dt * 100.0f;

		/** 如果粒子消失或生命结束 */
		if (position.y <= -1 || lifetime <= 0)
		{
			//respawnParticle(particle);
			//初始化颜色（白色）
			float x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, dec, rx, ry, rv;
			float r, g, b, a;
			r = 1.0f;
			g = 1.0f;
			b = 1.0f;
			a = 1.0f;
			particle.color = glm::vec4(r, g, b, a);

			//初始化坐标
			x = range_x * (rand() / float(RAND_MAX)) - range_x / 2;
			z = range_z * (rand() / float(RAND_MAX)) - range_z / 2;
			y = skyHeight;
			particle.position = glm::vec3(x, y, z);

			//初始化速度
			vx = rand() / float(RAND_MAX) *  0.05f;
			vy = rand() / float(RAND_MAX) * (0.1f - 0.05f) + 0.05f;
			vz = rand() / float(RAND_MAX) *  0.1f;
			particle.velocity = glm::vec3(vx, vy, vz);
			//旋转角度
			rx = (rand() / float(RAND_MAX)) * 360.0f;
			ry = (rand() / float(RAND_MAX))  * 360.0f;
			//旋转速度
			//rv = rand() / float(RAND_MAX) * 0.1f;

			particle.rotateAngle = glm::vec2(rx, ry);
			particle.rotateSpeed = rand() / float(RAND_MAX) * 0.1f;

			//初始化加速度
			ax = 0;
			ay = 100.0f;//G_ACCE;
			az = 0;
			particle.acceleration = glm::vec3(ax, ay, az);

			//初始化生命周期
			particle.lifetime = 100.0f;

			//消失速度
			dec = rand() / float(RAND_MAX) * 0.05f;
			particle.dec = dec;

			//初始化大小
			particle.scale = rand() / float(RAND_MAX) * (0.1f - 0.05f) + 0.05f;
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

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
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
