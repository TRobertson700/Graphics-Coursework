#include "camera.h"

glm::vec3 camera::moveForward(glm::vec3 pos, GLfloat angle, GLfloat d)
{
	return glm::vec3(pos.x + d*std::sin(rotation*DEG_TO_RADIAN), pos.y, pos.z - d*std::cos(rotation*DEG_TO_RADIAN));
}

glm::vec3 camera::moveRight(glm::vec3 pos, GLfloat angle, GLfloat d)
{
	return glm::vec3(pos.x + d*std::cos(rotation*DEG_TO_RADIAN), pos.y, pos.z - d*std::sin(rotation*DEG_TO_RADIAN));
}

camera::camera()
{
}


camera::~camera()
{
}
