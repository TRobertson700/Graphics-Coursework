#pragma once
#include "glm.hpp"

class AABB
{
public:
	AABB operator+(const glm::vec3& p) const
	{
		AABB result = (*this);
		result.vecMin = p + vecMin;
		result.vecMax = p + vecMax;

		return result;
	}


	glm::vec3 vecMin;
	glm::vec3 vecMax;
};

