#include "Collision.h"
#include "GameObject.h"
#include <windef.h>

bool Collision::ClipLine(GLuint d, const AABB& aabbBox, const glm::vec3& v0, const glm::vec3& v1, GLfloat& f_low, GLfloat& f_high)
{
	GLfloat f_dim_low, f_dim_high;

	// d represents the axis, 0 = x, 1 = y, 2 = z,  currently unaware of any glm unions.
	if (d == 0) { //x
		f_dim_low = (aabbBox.vecMin.x - v0.x) / (v1.x - v0.x);
		f_dim_high = (aabbBox.vecMax.x - v0.x) / (v1.x - v0.x);
	}
	else if (d == 1) { // y
		f_dim_low = (aabbBox.vecMin.y - v0.y) / (v1.y - v0.y);
		f_dim_high = (aabbBox.vecMax.y - v0.y) / (v1.y - v0.y);
	}
	else if (d == 2) { // z
		f_dim_low = (aabbBox.vecMin.z - v0.z) / (v1.z - v0.z);
		f_dim_high = (aabbBox.vecMax.z - v0.z) / (v1.z - v0.z);
	}

	if (f_dim_high < f_dim_low)
		std::swap(f_dim_high, f_dim_low);

	if (f_dim_high < f_low)
		return false;

	if (f_dim_low > f_high)
		return false;

	f_low = max(f_dim_low, f_low);
	f_high = min(f_dim_high, f_high);

	if (f_low > f_high)
		return false;

	return true;
}

bool Collision::lineAABBIntersection(const AABB& aabbBox, const glm::vec3& vBegin, const glm::vec3 vEnd, glm::vec3 vecIntersection, GLfloat fFraction)
{
	GLfloat f_low = 0;
	GLfloat f_high = 1;

	if (!ClipLine(0, aabbBox, vBegin, vEnd, f_low, f_high)) return false; //x
	if (!ClipLine(1, aabbBox, vBegin, vEnd, f_low, f_high)) return false; //y
	if (!ClipLine(2, aabbBox, vBegin, vEnd, f_low, f_high)) return false; //z

	glm::vec3 b = vEnd - vBegin;

	vecIntersection = vBegin + b * f_low;

	fFraction = f_low;

	return true;
}

bool Collision::AABBIntersection(const AABB& a, const AABB& b)
{
	for (int i = 0; i < 3; i++)
	{
		if (i == 0) { //for x
			if (a.vecMin.x > b.vecMax.x) {
				return false;
			}
			if (a.vecMax.x < b.vecMin.x) {
				return false;
			}
		}
		if (i == 1) { //for y
			if (a.vecMin.y > b.vecMax.y) {
				return false;
			}
			if (a.vecMax.y < b.vecMin.y) {
				return false;
			}
		}
		if (i == 2) { //for z
			if (a.vecMin.z > b.vecMax.z) {
				return false;
			}
			if (a.vecMax.z < b.vecMin.z) {
				return false;
			}
		}
	}

	return true;
}


//reaction against a plane
bool Collision::CollsionTestAgainstPlane(Collider* objA, Collider* objB)
{
	GLfloat fFraction = 1;

	glm::vec3 vBegin = dynamic_cast<GameObject*>(objA)->getPosition() + glm::vec3(0, 0, 0);
	glm::vec3 vEnd = dynamic_cast<GameObject*>(objA)->getPosition() + glm::vec3(0, 1, 0);

	glm::vec3 testIntersection;
	if (lineAABBIntersection(dynamic_cast<GameObject*>(objB)->getAABB() + dynamic_cast<GameObject*>(objB)->getPosition(), vBegin, vEnd, testIntersection, 0))
	{
		dynamic_cast<GameObject*>(objA)->setPosition(glm::vec3(dynamic_cast<GameObject*>(objA)->getPosition().x, dynamic_cast<GameObject*>(objA)->getPosition().y, dynamic_cast<GameObject*>(objA)->getPosition().z));
		return true;
	}
	else {
		dynamic_cast<GameObject*>(objA)->setPosition(glm::vec3(dynamic_cast<GameObject*>(objA)->getPosition().x, dynamic_cast<GameObject*>(objA)->getPosition().y - 0.35f, dynamic_cast<GameObject*>(objA)->getPosition().z));
		return false;
	}
}


//reaction against a box
bool Collision::CollsionTestAgainstBox(Collider* objA, Collider* objB)
{

}