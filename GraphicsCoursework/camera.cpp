#include "camera.h"
#include "gtc/matrix_transform.hpp"

#define DEG_TO_RADIAN 0.017453293

glm::mat4 Camera::draw(glm::mat4 camView, Player* player)
{
	camView = glm::lookAt(player->getEye(), player->getAt(), player->getUp());
	return camView;
}