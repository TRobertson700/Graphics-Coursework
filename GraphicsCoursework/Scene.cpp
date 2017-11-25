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

Scene::Scene()
{
	player = new Player();
	cam = new Camera();
	meshes[0] = new Mesh();
	meshes[1] = new Mesh();

	shaderProgram = Renderer::initShaders("phong-tex.vert", "phong-tex.frag");
	Renderer::setLight(shaderProgram, light);
	Renderer::setMaterial(shaderProgram, material);
	// set light attenuation shader uniforms
	GLuint uniformIndex = glGetUniformLocation(shaderProgram, "attConst");
	glUniform1f(uniformIndex, attConstant);
	uniformIndex = glGetUniformLocation(shaderProgram, "attLinear");
	glUniform1f(uniformIndex, attLinear);
	uniformIndex = glGetUniformLocation(shaderProgram, "attQuadratic");
	glUniform1f(uniformIndex, attQuadratic);

	meshes[0]->createMesh(cubeMeshID, "cube.obj");
	meshes[1]->createMesh(bunnyMeshID, "bunny-5000.obj");

	player->setMesh(meshes[0]);

}


void Scene::drawScene()
{
	glm::mat4 projection(1.0);
	projection = glm::perspective(float(60.0f * DEG_TO_RADIAN), 800.0f / 600.0f, 1.0f, 150.0f);

	glm::mat4 modelview(1.0);

	mvStack.push(modelview);
	mvStack.top() = cam->draw(mvStack.top(), player);
	
	glm::vec4 tmp = mvStack.top() * lightPos;
	light.position[0] = tmp.x;
	light.position[1] = tmp.y;
	light.position[2] = tmp.z;

	glUseProgram(shaderProgram);
	mvStack.push(mvStack.top());
	Renderer::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(projection));
	Renderer::setLightPos(shaderProgram, glm::value_ptr(tmp));

	mvStack.top() = player->draw(mvStack.top());

	Renderer::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	player->getMesh()->drawMesh(player->getMesh()->getMeshID());
	mvStack.pop();



	mvStack.pop();
}


void Scene::updateScene()
{
	player->update();
}