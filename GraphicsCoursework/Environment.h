#pragma once
#include "GameObject.h"

class Environment : public GameObject
{
public:
	Environment();
	void update();
	glm::mat4 draw(glm::mat4 modelMatrix);

	glm::vec3 getPosition() { return position; }
	glm::vec3 setPosition(glm::vec3 position) { return this->position = position; }


	AABB getAABB() { return box; }

private:
	glm::vec3 position;
	AABB box;
};

