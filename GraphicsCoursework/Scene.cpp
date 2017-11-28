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

GLfloat toonColour[4] = { 1.0, 0.2, 0.6, 1.0 };

Scene::Scene()
{
	player = new Player();
	cam = new Camera();
	meshes[0] =  Mesh();
	meshes[1] =  Mesh();
	shader = new Shader();
	ground = new Environment(glm::vec3(0, 0, 0), glm::vec3(25.0, 0.5, 25.0), 0, glm::vec3(0, 0, 1));
	wall = new Environment(glm::vec3(10, 5, 0), glm::vec3(1, 10, 10), 0, glm::vec3(0, 0, 1));
	collision = Collision();




	tProgram = Renderer::initShaders("toon.vert", "toon.frag");
	Renderer::setLight(tProgram, light);
	Renderer::setMaterial(tProgram, material);
	GLuint uniformIndex = glGetUniformLocation(tProgram, "colour.colour");
	glUniform4fv(uniformIndex, 1, toonColour);
	// set light attenuation shader uniforms
	shader->setAttenuation(tProgram, attConstant, attLinear, attQuadratic);

	pProgram = Renderer::initShaders("phong-tex.vert", "phong-tex.frag");
	Renderer::setLight(pProgram, light);
	Renderer::setMaterial(pProgram, material);
	// set light attenuation shader uniforms
	shader->setAttenuation(pProgram, attConstant, attLinear, attQuadratic);

	meshes[0].createMesh(cubeMeshID, "cube.obj");
	texture = Renderer::loadBitmap("sky.bmp");
	meshes[1].createMesh(bunnyMeshID, "bunny-5000.obj");

	dynamic_cast<Player*>(player)->setMesh(meshes[1]);
	dynamic_cast<Environment*>(ground)->setMesh(meshes[0]);
	dynamic_cast<Environment*>(wall)->setMesh(meshes[0]);

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

	shader->bindShaderProgram(tProgram);
	mvStack.push(mvStack.top());
	shader->useMatrix4fv(projection, "projection");
	mvStack.top() = player->draw(mvStack.top());
	shader->useMatrix4fv(mvStack.top(), "modelview");
	Renderer::setLightPos(tProgram, glm::value_ptr(tmp));
	shader->useMatrix3fv(glm::transpose(glm::inverse(glm::mat3(mvStack.top()))), "normalmatrix");
	dynamic_cast<Player*>(player)->getMesh().drawMesh(dynamic_cast<Player*>(player)->getMesh().getMeshID());
	mvStack.pop();
	shader->unbindShaderProgram();


	shader->bindShaderProgram(pProgram);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader->useMatrix4fv(projection, "projection");
	mvStack.push(mvStack.top());
	mvStack.top() = ground->draw(mvStack.top());
	shader->useMatrix4fv(mvStack.top(), "modelview");
	Renderer::setLightPos(pProgram, glm::value_ptr(tmp));
	dynamic_cast<Environment*>(ground)->getMesh().drawMesh(dynamic_cast<Environment*>(ground)->getMesh().getMeshID());
	mvStack.pop();

	//wall
	mvStack.push(mvStack.top());
	shader->useMatrix4fv(projection, "projection");
	mvStack.top() = wall->draw(mvStack.top());
	shader->useMatrix4fv(mvStack.top(), "modelview");
	Renderer::setLightPos(pProgram, glm::value_ptr(tmp));
	shader->useMatrix3fv(glm::transpose(glm::inverse(glm::mat3(mvStack.top()))), "normalmatrix");
	dynamic_cast<Environment*>(wall)->getMesh().drawMesh(dynamic_cast<Environment*>(wall)->getMesh().getMeshID());
	mvStack.pop();
	shader->unbindShaderProgram();


	mvStack.pop(); //initial matrix
}


void Scene::updateScene()
{
	player->update();
	collision.CollsionTestAgainstPlane(player, ground);
	collision.CollsionTestAgainstBox(player, wall);
	const Uint8* keys = SDL_GetKeyboardState(NULL);


	
}