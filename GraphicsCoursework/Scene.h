#pragma once
#include "player.h"

class Scene
{
public:
	Scene();

	void updateScene();
	void drawScene();

private:
	GLuint cubeMeshID, bunnyMeshID;
	Player* player;
	Mesh meshes[2];
};

