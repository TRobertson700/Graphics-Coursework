#pragma once
#include <glm.hpp>
#include <glew.h>
#include "player.h"

class Camera
{
public:
	Camera() {}

	glm::mat4 draw(glm::mat4 camView, Player* player);
};

