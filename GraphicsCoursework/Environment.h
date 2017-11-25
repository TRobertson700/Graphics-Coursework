#pragma once
#include "GameObject.h"

class Environment : public GameObject
{
public:
	Environment();
	void update();
	void draw();

	glm::vec3 getPosition() { return position; }
	glm::vec3 setPosition(glm::vec3 position) { return this->position = position; }


	AABB getAABB() { return box; }

private:
	glm::vec3 position;
	AABB box;
};

