#pragma once
#include <glm.hpp>
#include <glew.h>

#define DEG_TO_RADIAN 0.017453293

class camera
{
public:
	camera();
	~camera();	

	GLfloat rotation = 0.0f;

private:
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;
};

