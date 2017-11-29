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
	{ 0.4f, 0.4f, 0.4f, 1.0f }, // ambient
	{ 0.4f, 0.4f, 0.4f, 1.0f }, // diffuse
	{ 0.2f, 0.2f, 0.2f, 1.0f }, // specular
	2.0f  // shininess
};

Renderer::materialStruct tMaterial = {
	{ 0.2f, 0.2f, 0.2f, 1.0f }, // ambient
	{ 0.5f, 0.5f, 0.5f, 1.0f }, // diffuse
	{ 0.5f, 0.5f, 0.5f, 1.0f }, // specular
	2.0f  // shininess
};

glm::vec4 lightPos(0.0f, 10.0f, 0.0f, 1.0f); //light position

GLfloat attConstant = 0.05f;
GLfloat attLinear = 0.0f;
GLfloat attQuadratic = 0.0f;
GLuint texture[3];

GLfloat toonColour[4] = { 1.0, 1.0, 1.0, 1.0 };

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

	program[0] = Renderer::initShaders("toon.vert", "toon.frag");
	Renderer::setLight(program[0], light);
	Renderer::setMaterial(program[0], tMaterial);
	GLuint uniformIndex = glGetUniformLocation(program[0], "colour.colour");
	glUniform4fv(uniformIndex, 1, toonColour);
	// set light attenuation shader uniforms
	shader->setAttenuation(program[0], attConstant, attLinear, attQuadratic);

	program[1] = Renderer::initShaders("phong-tex.vert", "phong-tex.frag");
	Renderer::setLight(program[1], light);
	Renderer::setMaterial(program[1], material);
	// set light attenuation shader uniforms
	shader->setAttenuation(program[1], attConstant, attLinear, attQuadratic);

	program[2] = Renderer::initShaders("toonReflection.vert", "toonReflection.frag");
	Renderer::setLight(program[2], light);
	Renderer::setMaterial(program[2], material);
	// set light attenuation shader uniforms
	shader->setAttenuation(program[2], attConstant, attLinear, attQuadratic);
	// Binding tex handles to tex units to samplers under programmer control
	// set cubemap sampler to texture unit 1, arbitrarily
	uniformIndex = glGetUniformLocation(program[2], "textureUnit1");
	glUniform1i(uniformIndex, 1);
	// set tex sampler to texture unit 0, arbitrarily
	uniformIndex = glGetUniformLocation(program[2], "textureUnit0");
	glUniform1i(uniformIndex, 0);

	skyProgram = Renderer::initShaders("cubeMap.vert", "cubeMap.frag");

	const char *cubeTexFiles[6] = {
		"cloudy-skybox/back.bmp",
		"cloudy-skybox/front.bmp",
		"cloudy-skybox/right.bmp",
		"cloudy-skybox/left.bmp",
		"cloudy-skybox/up.bmp",
		"cloudy-skybox/down.bmp"
	};
	loadCubeMap(cubeTexFiles, &skybox[0]);

	meshes[0].createMesh(cubeMeshID, "cube.obj");
	texture[0] = Renderer::loadBitmap("sky.bmp");
	meshes[1].createMesh(bunnyMeshID, "bunny-5000.obj");
	texture[1] = Renderer::loadBitmap("bunny.bmp");

	dynamic_cast<Player*>(player)->setMesh(meshes[1]);
	dynamic_cast<Environment*>(ground)->setMesh(meshes[0]);
	dynamic_cast<Environment*>(wall)->setMesh(meshes[0]);

}

GLuint Scene::loadCubeMap(const char *fname[6], GLuint *texID)
{
	glGenTextures(1, texID); // generate texture ID
	GLenum sides[6] = { GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y };
	SDL_Surface *tmpSurface;

	glBindTexture(GL_TEXTURE_CUBE_MAP, *texID); // bind texture and set parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	GLuint externalFormat;
	for (int i = 0; i < 6; i++)
	{
		// load file - using core SDL library
		tmpSurface = SDL_LoadBMP(fname[i]);
		if (!tmpSurface)
		{
			std::cout << "Error loading bitmap" << std::endl;
			return *texID;
		}

		// skybox textures should not have alpha (assuming this is true!)
		SDL_PixelFormat *format = tmpSurface->format;
		externalFormat = (format->Rmask < format->Bmask) ? GL_RGB : GL_BGR;

		glTexImage2D(sides[i], 0, GL_RGB, tmpSurface->w, tmpSurface->h, 0,
			externalFormat, GL_UNSIGNED_BYTE, tmpSurface->pixels);
		// texture loaded, free the temporary buffer
		SDL_FreeSurface(tmpSurface);
	}
	return *texID;	// return value of texure ID, redundant really
}


void Scene::drawScene()
{
	glm::mat4 projection(1.0);
	projection = glm::perspective(float(60.0f * DEG_TO_RADIAN), 800.0f / 600.0f, 1.0f, 150.0f);


	glm::mat4 modelview(1.0);

	mvStack.push(modelview);
	mvStack.top() = cam->draw(mvStack.top(), dynamic_cast<Player*>(player));
	
	// skybox as single cube using cube map
	glCullFace(GL_FRONT);
	glUseProgram(skyProgram);
	Renderer::setUniformMatrix4fv(skyProgram, "projection", glm::value_ptr(projection));
	glDepthMask(GL_FALSE); // make sure writing to update depth test is off
	glm::mat3 mvRotOnlyMat3 = glm::mat3(mvStack.top());
	mvStack.push(glm::mat4(mvRotOnlyMat3));

	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox[0]);
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(1.5f, 1.5f, 1.5f));
	Renderer::setUniformMatrix4fv(skyProgram, "modelview", glm::value_ptr(mvStack.top()));
	meshes[0].drawMesh(meshes[0].getMeshID());
	mvStack.pop();
	glCullFace(GL_BACK);


	// back to remainder of rendering
	glDepthMask(GL_TRUE); // make sure depth test is on

	glm::vec4 tmp = mvStack.top() * lightPos;
	light.position[0] = tmp.x;
	light.position[1] = tmp.y;
	light.position[2] = tmp.z;

	shader->bindShaderProgram(program[currProgram]);
	shader->useMatrix4fv(projection, "projection");
	Renderer::setLightPos(program[currProgram], glm::value_ptr(tmp));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	glm::mat4 modelMatrix(1.0);
	mvStack.push(mvStack.top());
	modelMatrix = player->draw(modelMatrix);
	mvStack.top() *= modelMatrix;
	shader->useMatrix4fv(mvStack.top(), "modelview");
	shader->useMatrix4fv(modelMatrix, "modelMatrix");
	//shader->useMatrix3fv(glm::transpose(glm::inverse(glm::mat3(mvStack.top()))), "normalmatrix");
	GLuint uniformIndex = glGetUniformLocation(program[currProgram], "cameraPos");
	glUniform3fv(uniformIndex, 1, glm::value_ptr(dynamic_cast<Player*>(player)->getEye()));

	Renderer::setMaterial(program[currProgram], material);
	dynamic_cast<Player*>(player)->getMesh().drawMesh(dynamic_cast<Player*>(player)->getMesh().getMeshID());
	mvStack.pop();
	shader->unbindShaderProgram();


	glBindTexture(GL_TEXTURE_2D, texture[1]);
	shader->bindShaderProgram(program[1]);
	shader->useMatrix4fv(projection, "projection");
	mvStack.push(mvStack.top());
	mvStack.top() = ground->draw(mvStack.top());
	shader->useMatrix4fv(mvStack.top(), "modelview");
	Renderer::setLightPos(program[1], glm::value_ptr(tmp));
	dynamic_cast<Environment*>(ground)->getMesh().drawMesh(dynamic_cast<Environment*>(ground)->getMesh().getMeshID());
	mvStack.pop();

	//wall
	mvStack.push(mvStack.top());
	shader->useMatrix4fv(projection, "projection");
	mvStack.top() = wall->draw(mvStack.top());
	shader->useMatrix4fv(mvStack.top(), "modelview");
	Renderer::setLightPos(program[1], glm::value_ptr(tmp));
	dynamic_cast<Environment*>(wall)->getMesh().drawMesh(dynamic_cast<Environment*>(wall)->getMesh().getMeshID());
	mvStack.pop();
	shader->unbindShaderProgram();
	glBindTexture(GL_TEXTURE_2D, 0);


	mvStack.pop(); //initial matrix
}


void Scene::updateScene()
{
	player->update();
	dynamic_cast<Player*>(player)->setOnground(collision.CollsionTestAgainstPlane(player, ground));
	collision.CollsionTestAgainstBox(player, wall);
	const Uint8* keys = SDL_GetKeyboardState(NULL);


	if (keys[SDL_SCANCODE_1]) currProgram = 0;
	if (keys[SDL_SCANCODE_2]) currProgram = 1;
	if (keys[SDL_SCANCODE_3]) currProgram = 2;
}