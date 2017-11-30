#include "Environment.h"


Environment::Environment(glm::vec3 position, glm::vec3 scaled, GLfloat rotate, glm::vec3 rotateAxis)
{
	mesh = Mesh();
	box = AABB();

	this->position = position;
	this->scale = scaled;
	this->rotate = rotate;
	this->rotateAxis = rotateAxis;

	box.vecMin = glm::vec3(-scale.x, -scale.y, -scale.z);
	box.vecMax = glm::vec3(scale.x, scale.y, scale.z);
}

void Environment::update()
{

}


glm::mat4 Environment::draw(glm::mat4 modelMatrix)
{
	modelMatrix = mesh.meshTranslation(modelMatrix, position);
	modelMatrix = mesh.meshScaling(modelMatrix, scale);
	modelMatrix = mesh.meshRotation(modelMatrix, rotate, rotateAxis);
	return modelMatrix;
}
