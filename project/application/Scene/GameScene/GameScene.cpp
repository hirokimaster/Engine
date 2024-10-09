#include "GameScene.h"
#include "engine/ModelManager/ModelManager.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	texHandle = TextureManager::Load("resources/monsterBall.png");
	texHandle2 = TextureManager::Load("resources/white.png");
}

void GameScene::Update()
{
}

void GameScene::Draw()
{
}

void GameScene::PostProcessDraw()
{
}
