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
	GLuint loadCubeMap(const char *fname[6], GLuint *texID);

	GLuint cubeMeshID, bunnyMeshID, skyProgram;
	GLuint program[4];
	GLuint currProgram;
	GLuint skybox[5];
	GameObject* player;
	GameObject* ground;
	GameObject* wall, * wall2, * wall3, * wall4, *box;
	Camera* cam;
	Mesh meshes[2];
	Shader* shader;
	std::stack<glm::mat4> mvStack;
	Collision collision;


};

