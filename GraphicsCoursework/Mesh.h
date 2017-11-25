#pragma once
#include "Renderer.h"
#include "glm.hpp"

class Mesh
{
public:
	Mesh();
	~Mesh();

	GLuint createMesh(GLuint meshID, const char* filename);
	GLuint getIndexCount() { return indexCount; }
	void meshTranslation(glm::mat4 modelMatrix, glm::vec3 position);
	void meshScaling(glm::mat4 modelMatrix, glm::vec3 scale);
	void meshRotation(glm::mat4 modelMatrix, GLfloat roation, glm::vec3 rotate);
	void drawMesh(GLuint meshID, GLuint indexCount);

private:
	GLuint indexCount;
};

