#include "player.h"

#define DEG_TO_RADS 0.017453293

Player::Player()
{
	position = glm::vec3(5, 1, 4);
	eye = glm::vec3(0.0f, 1.0f, 10.0f); // left, up, forward
	at = glm::vec3(0.0f, 1.0f, 3.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	mesh = Mesh();

}

void Player::update()
{
	rotation += 0.1f;

	at = position;
	eye = moveForward(at, rotation, -10.0f);
	eye.y = position.y + 2;
}


glm::mat4 Player::draw(glm::mat4 modelMatrix)
{
	modelMatrix = mesh.meshTranslation(modelMatrix, position);
	modelMatrix = mesh.meshScaling(modelMatrix, glm::vec3(10, 10, 10));

	return modelMatrix;
}



//// moves the player either forwards or backwards.  
//// First parameter takes the position of the player
//// next is the angle to find where the player is facing
//// followed by a direction of where the player should move
//// anything above 0 will go forward, and less than 0 will move backwards.
glm::vec3 Player::moveForward(glm::vec3 pos, GLfloat angle, GLfloat d)
{
	return glm::vec3(pos.x + d*std::sin(rotation * DEG_TO_RADS), pos.y, pos.z - d*std::cos(rotation * DEG_TO_RADS));
}

//// moves the player either left or right.  
//// First parameter takes the position of the player
//// next is the angle to find where the player is facing
//// followed by a direction of where the player should move
//// anything above 0 will go right, and less than 0 will move left.
glm::vec3 Player::moveToSide(glm::vec3 pos, GLfloat angle, GLfloat d)
{
	return glm::vec3(pos.x + d*std::cos(rotation * DEG_TO_RADS), pos.y, pos.z + d*std::sin(rotation * DEG_TO_RADS));
}


void Player::inputHandler()
{
	keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_W]) position = moveForward(position, rotation, 0.1f);
	if (keys[SDL_SCANCODE_S]) position = moveForward(position, rotation, -0.1f);
	if (keys[SDL_SCANCODE_A]) position = moveToSide(position, rotation, -0.1f);
	if (keys[SDL_SCANCODE_D]) position = moveToSide(position, rotation, 0.1f);
}