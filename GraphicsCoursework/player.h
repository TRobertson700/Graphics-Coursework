#pragma once
#include <glew.h>
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();
	~Player();
	void update();
	void draw();

	glm::vec3 getPosition() { return position; }
	glm::vec3 setPosition(glm::vec3 position) { return this->position = position; }

	AABB getAABB() { return box; }
	
private:
	glm::vec3 position;
	AABB box;
};

