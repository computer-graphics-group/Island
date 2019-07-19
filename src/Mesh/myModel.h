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
	// 带有动画的模型
	MyModel();
	MyModel(glm::vec3 position, float  rotation, glm::vec3 size, glm::vec3 rotateDirction);
	void initAnimation(std::string modelPath, Shader skinning);
	static const uint MAX_BONES = 100;
	MyMesh m_mesh;
	GLuint m_boneLocation[MAX_BONES];
	void SetBoneTransform(uint Index, const Matrix4f& Transform);
	void UpdateBone(float runningTime);
	// 渲染 第二个参数是模型旋转的方向
	void Render(Shader shader, float scale = 0.0);
	// 物体位置
	glm::vec3   Position;
	// 物体旋转
	float     Rotation;
	// 物体大小
	glm::vec3   Size;
	vector<Matrix4f> Transforms;
	glm::vec3 rotateDirction;
};

#endif