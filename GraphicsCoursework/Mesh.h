#pragma once
#include "Renderer.h"
#include "glm.hpp"

class Mesh
{
public:
	Mesh() {}
	GLuint createMesh(GLuint meshID, const char* filename);
	GLuint getIndexCount() { return indexCount; }
	GLuint getMeshID() { return meshIndex; }
	glm::mat4 meshTranslation(glm::mat4 modelMatrix, glm::vec3 position);
	glm::mat4 meshScaling(glm::mat4 modelMatrix, glm::vec3 scale);
	glm::mat4 meshRotation(glm::mat4 modelMatrix, GLfloat roation, glm::vec3 rotate);
	void drawMesh(GLuint meshID);

private:
	GLuint indexCount;
	GLuint meshIndex;
};
