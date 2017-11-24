//Group Coursework Adanced Graphics Programming
//B00307642 and B00307619

#if _DEBUG
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif

#include "Renderer.h"
#include "camera.h"
#include <stack>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

GLuint shader, toonShader, skyboxShader;
GLuint indexCount = 0, cubeIndex = 0;
GLuint rabbit, cube;
GLuint texture;

std::stack<glm::mat4> mvStack;


Renderer::lightStruct light = {
	{ 0.3f, 0.3f, 0.3f, 1.0f }, // ambient
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // diffuse
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // specular
	{ 10.0f, 10.0f, 10.0f, 1.0f }  // position
};

glm::vec4 lightPos(-10.0f, 2.0f, 15.0f, 1.0f); //light position

Renderer::materialStruct material = {
	{ 0.2f, 0.4f, 0.2f, 1.0f }, // ambient
	{ 0.5f, 1.0f, 0.5f, 1.0f }, // diffuse
	{ 0.0f, 0.1f, 0.0f, 1.0f }, // specular
	2.0f  // shininess
};

void init()
{
	shader = Renderer::initShaders("phong-tex.vert", "phong-tex.frag");
	toonShader = Renderer::initShaders("toon.vert", "toon.frag");
	skyboxShader = Renderer::initShaders("cubeMap.vert", "cubeMap.frag");

	std::vector<GLfloat> verts, norms, texCoords;
	std::vector<GLuint> indices;

	Renderer::loadObj("bunny-5000.obj", verts, norms, texCoords, indices);
	GLuint size = indices.size();
	indexCount =size;
	rabbit = Renderer::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), nullptr, indexCount, indices.data());

	verts.clear();
	norms.clear();
	texCoords.clear();
	indices.clear();
	Renderer::loadObj("cube.obj", verts, norms, texCoords, indices);
	size = indices.size();
	cubeIndex = size;
	cube = Renderer::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), nullptr, cubeIndex, indices.data());

	texture = Renderer::loadBitmap("sky.bmp");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void update()
{

}

void draw(SDL_Window * window)
{
	// clear the screen
	glEnable(GL_CULL_FACE);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection(1.0);
	projection = glm::perspective(float(60.0f*DEG_TO_RADIAN), 800.0f / 600.0f, 1.0f, 150.0f);

	glm::mat4 modelview(1.0); // set base position for scene
	mvStack.push(modelview);

	glUseProgram(shader);
	Renderer::setUniformMatrix4fv(shader, "projection", glm::value_ptr(projection));

	glm::vec4 tmp = mvStack.top()*lightPos;
	light.position[0] = tmp.x;
	light.position[1] = tmp.y;
	light.position[2] = tmp.z;
	Renderer::setLightPos(shader, glm::value_ptr(tmp));

	// draw a cube for ground plane
	glBindTexture(GL_TEXTURE_2D, texture);
	mvStack.push(mvStack.top());
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-10.0f, -0.1f, -10.0f));
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(20.0f, 0.1f, 20.0f));
	Renderer::setUniformMatrix4fv(shader, "modelview", glm::value_ptr(mvStack.top()));
	Renderer::setMaterial(shader, material);
	Renderer::drawIndexedMesh(cube, cubeIndex, GL_TRIANGLES);
	mvStack.pop();

	//draws all the toon shaded, alternate toon shaded, and metallic toon shaded bunnies
	for (int i = 0; i < 7; i++)
	{
		glUseProgram(toonShader);
		Renderer::setLightPos(toonShader, glm::value_ptr(tmp));
		Renderer::setUniformMatrix4fv(toonShader, "projection", glm::value_ptr(projection));
		mvStack.push(mvStack.top());
		mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-25.0f + i + i * 4, -0.8f, 0.0f));
		mvStack.top() = glm::scale(mvStack.top(), glm::vec3(20.0f, 20.0f, 20.0f));
		Renderer::setUniformMatrix4fv(toonShader, "modelview", glm::value_ptr(mvStack.top()));
		Renderer::setMaterial(toonShader, material);
		Renderer::drawIndexedMesh(rabbit, indexCount, GL_TRIANGLES);
		mvStack.pop();
	}

	// remember to use at least one pop operation per push...
	mvStack.pop(); // initial matrix
	glDepthMask(GL_TRUE);

	SDL_GL_SwapWindow(window); // swap buffers
}

int main(int argc, char *argv[])
{
	SDL_Window * hWindow; // handles windows.
	SDL_GLContext glContext;
	hWindow = Renderer::createWindow(glContext);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cout << "glewInit failed, aborting." << std::endl;
		exit(1);
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	init();

	bool running = true;
	SDL_Event sdlEvent;
	while (running) {
		while (SDL_PollEvent(&sdlEvent)) {
			if (sdlEvent.type == SDL_QUIT) running = false;
		}

		update();
		draw(hWindow);
	}

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(hWindow);
	SDL_Quit();
	return 0;
}