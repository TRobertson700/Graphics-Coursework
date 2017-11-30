#pragma once
#include "glew.h"
#include "glm.hpp"
#include "Renderer.h"

class Shader
{
public:
	Shader() {} //does nothing
	GLuint createShader(const char* vert, const char* frag);
	GLuint createShader(const char* vert, const char* frag, Renderer::materialStruct material, Renderer::lightStruct light);
	GLuint setAttenuation(GLuint program, GLfloat attconst, GLfloat attlinear, GLfloat attquadratic);
	void bindShaderProgram(GLuint program);
	void unbindShaderProgram();
	void useMatrix4fv(glm::mat4 matrix, const char* uniform);
	void useMatrix3fv(glm::mat3 matrix, const char* uniform);

private:
	GLuint currentProgram;
};

