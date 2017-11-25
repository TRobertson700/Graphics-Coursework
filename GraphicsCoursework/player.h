#pragma once
#include <glew.h>
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();
	void update();
	void draw();

	glm::vec3 getPosition() { return position; }
	glm::vec3 setPosition(glm::vec3 position) { return this->position = position; }

	glm::mat4 getModelMatrix() { return modelMatrix; }

	Mesh getMesh() { return mesh; }
	Mesh setMesh(Mesh mesh) { return this->mesh = mesh; }

	AABB getAABB() { return box; }
	
private:
	glm::vec3 position;
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;

	glm::mat4 modelMatrix;

	Mesh mesh;

	GLfloat rotation;
	const Uint8 *keys;
	AABB box;

	glm::vec3 moveForward(glm::vec3 pos, GLfloat angle, GLfloat d);
	glm::vec3 moveToSide(glm::vec3 pos, GLfloat angle, GLfloat d);
	void inputHandler();
};
