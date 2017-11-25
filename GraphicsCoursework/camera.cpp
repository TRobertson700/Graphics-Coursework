#include "camera.h"

glm::vec3 camera::moveForward(glm::vec3 pos, GLfloat angle, GLfloat d)
{
	return glm::vec3(pos.x + d*std::sin(rotation*DEG_TO_RADIAN), pos.y, pos.z - d*std::cos(rotation*DEG_TO_RADIAN));
}

glm::vec3 camera::moveRight(glm::vec3 pos, GLfloat angle, GLfloat d)
{
	return glm::vec3(pos.x + d*std::cos(rotation*DEG_TO_RADIAN), pos.y, pos.z - d*std::sin(rotation*DEG_TO_RADIAN));
}

glm::mat4 camera::draw(glm::mat4 camera, GLuint shader)
{
	glm::mat4 projection(1.0);
	projection = glm::perspective(float(60.0f*DEG_TO_RADIAN), 800.0f / 600.0f, 1.0f, 500.0f);
	Renderer::setUniformMatrix4fv(shader, "projection", glm::value_ptr(projection));
	
	camera = glm::lookAt(player1->getEye(), playe1r->getAt(), player1->getUp());
	return camera;
}

camera::camera()
{
}


camera::~camera()
{
}
