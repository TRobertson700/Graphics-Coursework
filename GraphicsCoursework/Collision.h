#pragma once
#include "Collider.h"
#include "AABB.h"
#include "glew.h"

class Collision
{
public:
	Collision() {}

	bool ClipLine(GLuint d, const AABB& aabbBox, const glm::vec3& v0, const glm::vec3& v1, GLfloat& f_low, GLfloat& f_high);
	bool lineAABBIntersection(const AABB& aabbBox, const glm::vec3& vBegin, const glm::vec3 vEnd, glm::vec3 vecIntersection, GLfloat fFraction);
	bool CollsionTestAgainstPlane(Collider* objA, Collider* objB);
	bool CollsionTestAgainstBox(Collider* objA, Collider* objB);
	bool AABBIntersection(const AABB& a, const AABB& b);
};

