#include <cmath>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <learnopengl/shader.h>
#include <stb_image.h>
#include <learnopengl/filesystem.h>
using namespace std;

struct Particle {
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec2 rotateAngle;
	float rotateSpeed;
	float lifetime;
	float scale;
	float dec;
};


class ParticleSystem {
private:
	glm::vec3 GenPos;
	float range_x, range_z, skyHeight;
	int number;
	vector<Particle> particles;
	float lifetime, dec;
	glm::vec3 position, velocity, acceleration, scale;
	unsigned int VAO;
public:
	ParticleSystem(int number, glm::vec3 genPosition, float range_x, float range_z, float skyHeight);
	void initParticleSystem();
	void Update(float dt, glm::vec3 pos);
	void Draw(Shader &shader);

};
