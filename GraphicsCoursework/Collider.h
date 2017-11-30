#pragma once
#include "glm.hpp"

class Collider
{
public:
	Collider() {}
	virtual ~Collider() {}
	virtual void update() = 0;
	virtual glm::vec3 getPosition() = 0;
	virtual glm::vec3 setPosition(glm::vec3 position) = 0;
};