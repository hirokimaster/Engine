#include "ModelResources.h"

ModelResources* ModelResources::GetInstance()
{
	static ModelResources instance;
	return &instance;
}

void ModelResources::LoadModel()
{
	ModelManager::GetInstance()->LoadObjModel("cube.obj");
}