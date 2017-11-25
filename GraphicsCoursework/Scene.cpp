#include "Scene.h"



Scene::Scene()
{
	player = new Player();
	meshes[0] = Mesh();
	meshes[1] = Mesh();

	meshes[0].createMesh(cubeMeshID, "cube.obj");
	meshes[1].createMesh(bunnyMeshID, "bunny-5000");

	player->setMesh(meshes[0]);
}


void Scene::drawScene()
{
	player->getMesh().meshTranslation(player->getModelMatrix(), player->getPosition());
	player->getMesh().drawMesh(player->getMesh().getMeshID());
}


void Scene::updateScene()
{

}