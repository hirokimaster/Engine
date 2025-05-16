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
	ModelManager::GetInstance()->LoadObjModel("LevelEditorObj/grounds.obj");
	ModelManager::GetInstance()->LoadObjModel("LevelEditorObj/part.obj");
	ModelManager::GetInstance()->LoadObjModel("Enemy/enemy.obj");
	ModelManager::GetInstance()->LoadObjModel("LevelEditorObj/fixedEnemy.obj");
	ModelManager::GetInstance()->LoadObjModel("Player/bullet.obj");
	ModelManager::GetInstance()->LoadObjModel("Stage/wall.obj");
	ModelManager::GetInstance()->LoadObjModel("Stage/laser.obj");
	ModelManager::GetInstance()->LoadObjModel("LevelEditorObj/roads.obj");
	ModelManager::GetInstance()->LoadObjModel("LevelEditorObj/floor.obj");
	ModelManager::GetInstance()->LoadObjModel("Player/ball.obj");
}