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
	postProcess_ = std::make_unique<PostProcess>();
	postProcess_->Initialize();
	postProcess_->SetEffect(Grayscale);
	texHandle_ = TextureManager::Load("resources/taiyou.jpg");
	sprite_.reset(Sprite::Create(texHandle_));

}

void GameScene::Update()
{
	static int currentItem_ = 0;
	const char* item[4] = { "Bloom", "Vignette", "Grayscale","Gaussian" };
	ImGui::Begin("effect");
	if (ImGui::Combo("LightingType", &currentItem_, item, IM_ARRAYSIZE(item))) {
		if (currentItem_ == 0) {
			postProcess_->SetEffect(Bloom);
		}
		else if (currentItem_ == 1) {
			postProcess_->SetEffect(Vignette);
		}
		else if (currentItem_ == 2) {
			postProcess_->SetEffect(Grayscale);
		}
		else if (currentItem_ == 3) {
			postProcess_->SetEffect(GaussianBlur);
		}

	}
	ImGui::End();
}

void GameScene::Draw()
{
	postProcess_->Draw();
}

void GameScene::PostProcessDraw()
{
	postProcess_->PreDraw();

	sprite_->Draw();

	postProcess_->PostDraw();
}
