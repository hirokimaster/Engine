/**
* @file ModelResources.cpp
* @brief モデルの読み込みをまとめる
* @author 仁平 琉乃
*/

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
	ModelManager::GetInstance()->LoadObjModel("Player/player.obj");
	ModelManager::GetInstance()->LoadObjModel("Player/plane.obj");
	ModelManager::GetInstance()->LoadObjModel("skydome/skydome.obj");
	ModelManager::GetInstance()->LoadObjModel("LevelEditorObj/mounts2.obj");
}