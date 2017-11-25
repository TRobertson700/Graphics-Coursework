#pragma once
#include "player.h"
#include "Renderer.h"
#include <glm.hpp>
#include <glew.h>
#include <SDL.h>

#define DEG_TO_RADIAN 0.017453293

class camera
{
public:
	camera();
	~camera();	

	GLfloat rotation = 0.0f;

	glm::vec3 moveForward(glm::vec3 pos, GLfloat angle, GLfloat d);
	glm::vec3 moveRight(glm::vec3 pos, GLfloat angle, GLfloat d);
	glm::mat4 draw(glm::mat4 camera, GLuint shader);

private:
	player* player1;
	GLuint mouse;

	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;
};

