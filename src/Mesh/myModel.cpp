#include "myModel.h"
#include <iostream>

MyModel::MyModel()
{
}


MyModel::MyModel(glm::vec3 position, float  rotation, glm::vec3 size, glm::vec3 rotateDirction)
{
	Position = position;
	Rotation = rotation;
	Size = size;
	this->rotateDirction = rotateDirction;
}

void MyModel::initAnimation(std::string modelPath, Shader skinning)
{
	// ���ض���
	if (!m_mesh.LoadMesh(modelPath)) {
		std::cout << modelPath << " Mesh load failed\n";
	}

	// �󶨹���
	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation); i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		SNPRINTF(Name, sizeof(Name), "gBones[%d]", i);
		GLuint Location = glGetUniformLocation(skinning.ID, Name);
		if (Location == INVALID_UNIFORM_LOCATION) {
			fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", Name);
		}
		m_boneLocation[i] = Location;
	}
}

void MyModel::SetBoneTransform(uint Index, const Matrix4f& Transform)
{
	// ���ù�����λ��
	glUniformMatrix4fv(m_boneLocation[Index], 1, GL_TRUE, (const GLfloat*)Transform.m);

}
//���¹�����λ��
void MyModel::UpdateBone(float runningTime)
{
	vector<Matrix4f> Transforms;
	m_mesh.BoneTransform(runningTime, Transforms);
	for (uint i = 0; i < Transforms.size(); i++) {
		SetBoneTransform(i, Transforms[i]);
	}
}

void MyModel::Render(Shader shader, float scale)
{
	shader.use();
	// λ�� ���� ��ת
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Position);
	model = glm::rotate(model, -0.25f, glm::vec3(0.0f,1.0f, 0.0f));
	if (scale == 0.0f)
		model = glm::scale(model, Size);
	else
		model = glm::scale(model, glm::vec3(Size.x * scale, Size.y * scale, Size.z * scale));

	model = glm::scale(model, glm::vec3(10));
	// ��������ֵ
	shader.setMat4("model", model);
	// ÿ��mesh��Ⱦ
	m_mesh.Render();
}