#include "Scene.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#define DEG_TO_RADIAN 0.017453293

Renderer::lightStruct light = {
	{ 0.3f, 0.3f, 0.3f, 1.0f }, // ambient
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // diffuse
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // specular
	{ 10.0f, 10.0f, 10.0f, 1.0f }  // position
};


Renderer::materialStruct material = {
	{ 0.2f, 0.4f, 0.2f, 1.0f }, // ambient
	{ 0.5f, 1.0f, 0.5f, 1.0f }, // diffuse
	{ 0.0f, 0.1f, 0.0f, 1.0f }, // specular
	2.0f  // shininess
};

glm::vec4 lightPos(0.0f, 10.0f, 0.0f, 1.0f); //light position

GLfloat attConstant = 0.05f;
GLfloat attLinear = 0.0f;
GLfloat attQuadratic = 0.0f;
GLuint texture;

Scene::Scene()
{
	player = new Player();
	cam = new Camera();
	meshes[0] =  Mesh();
	meshes[1] =  Mesh();
	shader = new Shader();

	shaderProgram = Renderer::initShaders("toon.vert", "toon.frag");
	Renderer::setLight(shaderProgram, light);
	Renderer::setMaterial(shaderProgram, material);
	// set light attenuation shader uniforms
	shader->setAttenuation(shaderProgram, attConstant, attLinear, attQuadratic);


	meshes[0].createMesh(cubeMeshID, "cube.obj");
	texture = Renderer::loadBitmap("sky.bmp");
	meshes[1].createMesh(bunnyMeshID, "bunny-5000.obj");

	dynamic_cast<Player*>(player)->setMesh(meshes[1]);

}


void Scene::drawScene()
{
	glm::mat4 projection(1.0);
	projection = glm::perspective(float(60.0f * DEG_TO_RADIAN), 800.0f / 600.0f, 1.0f, 150.0f);

	glm::mat4 modelview(1.0);

	mvStack.push(modelview);
	mvStack.top() = cam->draw(mvStack.top(), dynamic_cast<Player*>(player));
	
	glm::vec4 tmp = mvStack.top() * lightPos;
	light.position[0] = tmp.x;
	light.position[1] = tmp.y;
	light.position[2] = tmp.z;

	shader->bindShaderProgram(shaderProgram);
	glBindTexture(GL_TEXTURE_2D, texture);
	mvStack.push(mvStack.top());
	shader->useMatrix4fv(projection, "projection");
	Renderer::setLightPos(shaderProgram, glm::value_ptr(tmp));

	mvStack.top() = player->draw(mvStack.top());

	shader->useMatrix4fv(mvStack.top(), "modelview");
	dynamic_cast<Player*>(player)->getMesh().drawMesh(dynamic_cast<Player*>(player)->getMesh().getMeshID());
	mvStack.pop();

	shader->unbindShaderProgram();

	mvStack.pop();
}


void Scene::updateScene()
{
	player->update();
}