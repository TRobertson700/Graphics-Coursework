#pragma once
#include "Camera.h"
#include <stack>

class Scene
{
public:
	Scene();

	void updateScene();
	void drawScene();

private:
	GLuint cubeMeshID, bunnyMeshID, shaderProgram;
	Player* player;
	Camera* cam;
	Mesh* meshes[2];
	std::stack<glm::mat4> mvStack;
};

