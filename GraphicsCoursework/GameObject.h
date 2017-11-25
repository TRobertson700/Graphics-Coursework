#pragma once
#include "Mesh.h"
#include "Collider.h"
#include "AABB.h"

class GameObject : public Collider
{
public:
	GameObject() { }
	virtual ~GameObject() {}
	virtual void update() = 0;
	virtual void draw() = 0;

	virtual glm::vec3 getPosition() = 0;
	virtual glm::vec3 setPosition(glm::vec3 position) = 0;

	virtual AABB getAABB() = 0;
};