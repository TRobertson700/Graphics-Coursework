#include "Shader.h"
#include "gtc\type_ptr.hpp"

////initialises the shader without material or lights
GLuint Shader::createShader(const char* vert, const char* frag)
{
	GLuint program = Renderer::initShaders(vert, frag);

	return program;
}

////initialises the shader with materials and lights
GLuint Shader::createShader(const char* vert, const char* frag, Renderer::materialStruct material, Renderer::lightStruct light)
{
	GLuint program = Renderer::initShaders(vert, frag);
	Renderer::setLight(program, light);
	Renderer::setMaterial(program, material);

	return program;
}

//// sets attenuation for the chosen shader, uniforms to use are labelled "attConst", "attLinear", "attQuadratic".
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

////bind shader program, updates the current program, call before setting uniform matrices
void Shader::bindShaderProgram(GLuint program)
{
	currentProgram = program;
	glUseProgram(currentProgram);
}

////unbind shader program, call when you before you use another shader.
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