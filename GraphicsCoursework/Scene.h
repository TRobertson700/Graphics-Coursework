#pragma once
#include "Camera.h"
#include "Shader.h"
#include <stack>

class Scene
{
public:
	Scene();

	void updateScene();
	void drawScene();

private:
	GLuint cubeMeshID, bunnyMeshID, shaderProgram;
	GameObject* player;
	Camera* cam;
	Mesh meshes[2];
	Shader* shader;
	std::stack<glm::mat4> mvStack;
};

