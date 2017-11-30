#pragma once
#include <glew.h>
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();
	void update();
	glm::mat4 draw(glm::mat4 modelMatrix);

	glm::vec3 getPosition() { return position; }
	glm::vec3 setPosition(glm::vec3 position) { return this->position = position; }


	Mesh getMesh() { return mesh; }
	Mesh setMesh(Mesh mesh) { return this->mesh = mesh; }

	glm::vec3 getEye() { return eye; }
	glm::vec3 getAt() { return at; }
	glm::vec3 getUp() { return up; }

	bool setOnground(bool onground) { return this->onground = onground; }

	AABB getAABB() { return box; }

private:
	glm::vec3 position, velocity;
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;

	Mesh mesh;

	GLfloat rotation;
	const Uint8 *keys;
	GLuint mouse;
	bool onground = false;
	AABB box;

	glm::vec3 moveForward(glm::vec3 pos, GLfloat angle, GLfloat d);
	glm::vec3 moveToSide(glm::vec3 pos, GLfloat angle, GLfloat d);
	void inputHandler();
	void MouseMotion(GLuint x, GLuint y);
};
