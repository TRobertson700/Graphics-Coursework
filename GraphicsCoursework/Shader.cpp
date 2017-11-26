#include "Shader.h"
#include "Renderer.h"
#include "gtc\type_ptr.hpp"


//// when creating the uniform float for the attenuation in the fragment file, make sure they are labeled the same
//// as the strings below in the glGetUniformLocation function.
GLuint Shader::setAttenuation(GLuint program, GLfloat attconst, GLfloat attlinear, GLfloat attquadratic)
{
	GLuint uniformIndex = glGetUniformLocation(program, "attConst");
	glUniform1f(uniformIndex, attconst);
	uniformIndex = glGetUniformLocation(program, "attLinear");
	glUniform1f(uniformIndex, attlinear);
	uniformIndex = glGetUniformLocation(program, "attQuadratic");
	glUniform1f(uniformIndex, attquadratic);

	return uniformIndex;
}

//bind shader program, updates the current program, call before setting uniform matrices
void Shader::bindShaderProgram(GLuint program)
{
	currentProgram = program;
	glUseProgram(currentProgram);
}

//unbind shader program, call when you before you use another shader.
void Shader::unbindShaderProgram()
{
	currentProgram = 0;
	glUseProgram(currentProgram);
}

////Sets up mat4 uniform matrix
void Shader::useMatrix4fv(glm::mat4 matrix, const char* uniform)
{
	Renderer::setUniformMatrix4fv(currentProgram, uniform, glm::value_ptr(matrix));
}

////Sets up mat3 uniform matrix
void Shader::useMatrix3fv(glm::mat3 matrix, const char* uniform)
{
	GLuint uniformIndex = glGetUniformLocation(currentProgram, uniform);
	auto data = glm::value_ptr(matrix);
	glUniformMatrix3fv(uniformIndex, 1, GL_FALSE, data);
}