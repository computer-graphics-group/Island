#ifndef MYMODEL_H
#define MYMODEL_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "mesh.h"
#include <learnopengl/shader.h>
#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF
class MyModel
{
public:
	// ���ж�����ģ��
	MyModel();
	MyModel(glm::vec3 position, float  rotation, glm::vec3 size, glm::vec3 rotateDirction);
	void initAnimation(std::string modelPath, Shader skinning);
	static const uint MAX_BONES = 100;
	MyMesh m_mesh;
	GLuint m_boneLocation[MAX_BONES];
	void SetBoneTransform(uint Index, const Matrix4f& Transform);
	void UpdateBone(float runningTime);
	// ��Ⱦ �ڶ���������ģ����ת�ķ���
	void Render(Shader shader, float scale = 0.0);
	// ����λ��
	glm::vec3   Position;
	// ������ת
	float     Rotation;
	// �����С
	glm::vec3   Size;
	vector<Matrix4f> Transforms;
	glm::vec3 rotateDirction;
};

#endif