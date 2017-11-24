#pragma once
//code based on the RT3D classes from the Advanced Graphics class and code found here:
//https://sourceforge.net/p/irrext/code/91/tree/trunk/extensions/scene/IMeshLoader/fbx/ and
//https://github.com/BSVino/MathForGameDevelopers/tree/shader-rimlight/renderer


#include <glew.h>
#include <SDL.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

#define RT3D_VERTEX		0
#define RT3D_COLOUR		1
#define RT3D_NORMAL		2
#define RT3D_TEXCOORD   3
#define RT3D_INDEX		4

#define WINDOW_HEIGHT  600
#define WINDOW_WIDTH  800

namespace Renderer 
{
	struct lightStruct {
		GLfloat ambient[4];
		GLfloat diffuse[4];
		GLfloat specular[4];
		GLfloat position[4];
	};

	struct materialStruct {
		GLfloat ambient[4];
		GLfloat diffuse[4];
		GLfloat specular[4];
		GLfloat shininess;
	};

	//for setting a colour to the toon shader
	struct colourStruct
	{
		GLfloat colour[4]; //values for red, green, blue and transparency
	};

	SDL_Window* createWindow(SDL_GLContext &context);
	void exitFatalError(const char *message);
	char* loadFile(const char *fname, GLint &fSize);
	void printShaderError(const GLint shader);
	GLuint initShaders(const char *vertFile, const char *fragFile);
	// Some methods for creating meshes
	// ... including one for dealing with indexed meshes
	GLuint createMesh(const GLuint numVerts, const GLfloat* vertices, const GLfloat* colours, const GLfloat* normals,
		const GLfloat* texcoords, const GLuint indexCount, const GLuint* indices);
	// these three create mesh functions simply provide more basic access to the full version
	GLuint createMesh(const GLuint numVerts, const GLfloat* vertices, const GLfloat* colours, const GLfloat* normals,
		const GLfloat* texcoords);
	GLuint createMesh(const GLuint numVerts, const GLfloat* vertices);
	GLuint createColourMesh(const GLuint numVerts, const GLfloat* vertices, const GLfloat* colours);

	void setUniformMatrix4fv(const GLuint program, const char* uniformName, const GLfloat *data);

	void setLight(const GLuint program, const lightStruct light);
	void setLightPos(const GLuint program, const GLfloat *lightPos);
	void setMaterial(const GLuint program, const materialStruct material);

	void drawMesh(const GLuint mesh, const GLuint numVerts, const GLuint primitive);
	void drawIndexedMesh(const GLuint mesh, const GLuint indexCount, const GLuint primitive);

	void updateMesh(const GLuint mesh, const unsigned int bufferType, const GLfloat *data, const GLuint size);

	void loadObj(const char* filename, std::vector<GLfloat> &verts, std::vector<GLfloat> &norms, std::vector<GLfloat> &texcoords, std::vector<GLuint> &indices);
	GLuint loadBitmap(char * name);
}