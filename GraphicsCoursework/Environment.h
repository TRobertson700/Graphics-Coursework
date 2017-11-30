#pragma once
#include "GameObject.h"

class Environment : public GameObject
{
public:
	Environment() {}
	Environment(glm::vec3 position, glm::vec3 scaled, GLfloat rotate, glm::vec3 rotateAxis);
	void update();
	glm::mat4 draw(glm::mat4 modelMatrix);

	glm::vec3 getPosition() { return position; }
	glm::vec3 setPosition(glm::vec3 position) { return this->position = position; }

	Mesh getMesh() { return mesh; }
	Mesh setMesh(Mesh mesh) { return this->mesh = mesh; }

	AABB getAABB() { return box; }

private:
	glm::vec3 position;
	glm::vec3 scale;
	GLfloat rotate;
	glm::vec3 rotateAxis;
	Mesh mesh;
	AABB box;
};

