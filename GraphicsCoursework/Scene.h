#pragma once
#include "Camera.h"
#include "Shader.h"
#include <stack>
#include "Environment.h"
#include "Collision.h"

class Scene
{
public:
	Scene();

	void updateScene();
	void drawScene();

private:
	GLuint cubeMeshID, bunnyMeshID, tProgram, pProgram;
	GameObject* player;
	GameObject* ground;
	GameObject* wall;
	Camera* cam;
	Mesh meshes[2];
	Shader* shader;
	std::stack<glm::mat4> mvStack;
	Collision collision;
};

