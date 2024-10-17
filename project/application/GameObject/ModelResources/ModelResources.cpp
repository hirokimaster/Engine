#include "ModelResources.h"

ModelResources* ModelResources::GetInstance()
{
	static ModelResources instance;
	return &instance;
}

void ModelResources::LoadModel()
{
	ModelManager::GetInstance()->LoadObjModel("Player/cube.obj");
	ModelManager::GetInstance()->LoadObjModel("Enemy/cube.obj");
	ModelManager::GetInstance()->LoadObjModel("Player/Jet.obj");
	ModelManager::GetInstance()->LoadObjModel("skydome/skydome.obj");
	ModelManager::GetInstance()->LoadObjModel("Player/plane.obj");
}