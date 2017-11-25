#pragma once
#include <glew.h>
#include <SDL.h>

class player
{
public:
	player();
	~player();
	void render();
	void update();

private:
	GLuint model, texture;
	GLuint mouse;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
};

