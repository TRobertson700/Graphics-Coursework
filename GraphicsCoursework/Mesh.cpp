#include "Mesh.h"
#include <iostream>
#include <vector>
#include "gtc\type_ptr.hpp"
#include "gtc\matrix_transform.hpp"

#define DEG_TO_RADIAN 0.017453293

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

////Function only supports loading in obj files, meshID is used to create the mesh data, file name is the name of the obj file.
////Function returns the indexCount which are the size of the indices.
GLuint Mesh::createMesh(GLuint meshID, const char* filename)
{
	std::vector<GLfloat> verts;
	std::vector<GLfloat> norms;
	std::vector<GLfloat> tex_coords;
	std::vector<GLuint> indices;

	Renderer::loadObj(filename, verts, norms, tex_coords, indices);
	indexCount = indices.size();
	meshID = Renderer::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), indexCount, indices.data());

	return indexCount;
}

////Function takes in the model matrix and vec3 position.
void Mesh::meshTranslation(glm::mat4 modelMatrix, glm::vec3 position)
{
	modelMatrix = glm::translate(modelMatrix, position);
}

////Function takes in model matrix and a vec3 scaler.
////anything above the value 1 will be increase in size of the model in the axis appropriate.
////Anything below the value 1 will decrease the size of the model in the axis appropriate.
////Pass in any value that is higher than zero.
void Mesh::meshScaling(glm::mat4 modelMatrix, glm::vec3 scale)
{
	modelMatrix = glm::scale(modelMatrix, scale);
}

////Function takes the model matrix and a rotation in degrees and a vec3 rotate for the axis to rotate around.
////Anything higher than or less than 0 in the vec3 parameter will result in that axis being rotated.
////if value is 0 there will be no rotation in that axis.
void Mesh::meshRotation(glm::mat4 modelMatrix, GLfloat rotation, glm::vec3 rotate)
{
	modelMatrix = glm::rotate(modelMatrix, float(rotation * DEG_TO_RADIAN), rotate);
}

void Mesh::drawMesh(GLuint meshID, GLuint indexCount)
{
	Renderer::drawIndexedMesh(meshID, indexCount, GL_TRIANGLES);
}