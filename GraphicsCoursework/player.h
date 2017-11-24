#pragma once
#include <glew.h>

class player
{
public:
	player();
	~player();
	void render();
	void update();

private:
	GLuint model, texture;
	
};

