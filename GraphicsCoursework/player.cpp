#include "player.h"

#define DEG_TO_RADS 0.017453293

Player::Player()
{
	position = glm::vec3(5, 10, 4);
	eye = glm::vec3(0.0f, 1.0f, 10.0f); // left, up, forward
	at = glm::vec3(0.0f, 1.0f, 3.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	box.vecMin = glm::vec3(-1, -1, -1);
	box.vecMax = glm::vec3(0.5, 0.5, 0.5);

	mesh = Mesh();
}

void Player::MouseMotion(GLuint x, GLuint y)
{
	//if (fov > 10) fov = 10;
	//else if (fov < 2) fov = 2;

	//if (mouse & SDL_BUTTON(SDL_BUTTON_LEFT)) fov += 0.05f;
	//if (mouse & SDL_BUTTON(SDL_BUTTON_RIGHT)) fov -= 0.05f;
	if (mouse & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		rotation = x;
	}
}

void Player::update()
{
	at = position;
	eye = moveForward(at, rotation, -10.0f);
	eye.y = position.y + 2;

	inputHandler();
}


glm::mat4 Player::draw(glm::mat4 modelMatrix)
{
	modelMatrix = mesh.meshTranslation(modelMatrix, position);
	modelMatrix = mesh.meshScaling(modelMatrix, glm::vec3(20, 20, 20));
	modelMatrix = mesh.meshRotation(modelMatrix, -90 + rotation, glm::vec3(0, 1, 0));
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
	int x, y;
	position += velocity;
	velocity *= 0.89f;
	mouse = SDL_GetMouseState(&x, &y);
	keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_W] && onground) {
		position = moveForward(position, rotation, 0.1f);
		//velocity.y += 0.5;
	}
	if (keys[SDL_SCANCODE_S]) position = moveForward(position, rotation, -0.1f);
	if (keys[SDL_SCANCODE_A]) position = moveToSide(position, rotation, -0.1f);
	if (keys[SDL_SCANCODE_D]) position = moveToSide(position, rotation, 0.1f);

	MouseMotion(x, y);
}