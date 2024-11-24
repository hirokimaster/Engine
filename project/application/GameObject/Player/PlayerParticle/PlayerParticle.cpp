/**
* @file PlayerParticle.cpp
* @brief プレイヤー（自機）のパーティクル
* @author 仁平 琉乃
*/

#include "PlayerParticle.h"

void PlayerParticle::Initialize()
{
	texHandle_ = TextureManager::Load("resources/Player/smoke.png");
	particle_ = std::make_unique<GPUParticle>();
	particle_->SetModel("Player/plane.obj");
	particle_->Initialize();
	particle_->SetTexHandle(texHandle_);
	// particleの値
	param_.count = 50;
	param_.frequency = 0.1f;
	param_.frequencyTime = 0.0f;
	param_.radius = 1.0f;
	param_.emit = 0;
	param_.rangeTranslate.min = Vector3(0.0f, 0.0f, 0.0f);
	param_.rangeTranslate.max = Vector3(0.0f, 0.0f, 0.0f);
	param_.rangeScale.min = Vector3(0.4f, 0.4f, 0.4f);
	param_.rangeScale.max = Vector3(0.5f, 0.5f, 0.5f);
	param_.rangeLifeTime.min = 0.5f;
	param_.rangeLifeTime.max = 1.0f;
	param_.rangeVelocity.min = Vector3(0.0f, 0.0f, -1.0f);
	param_.rangeVelocity.max = Vector3(0.0f, 0.0f, -3.0f);
	param_.rangeCurrentTime.min = 0.0f;
	param_.rangeCurrentTime.max = 5.0f;
	param_.rangeColor.min = Vector3(1.0f, 1.0f, 1.0f);
	param_.rangeColor.max = Vector3(1.0f, 1.0f, 1.0f);
	param_.rangeAlpha.min = 0.6f;
	param_.rangeAlpha.max = 1.0f;

	particle_->SetParticleParam(param_);

}

void PlayerParticle::Update()
{
	param_.translate = position_;
	param_.frequencyTime += 1.0f / 60.0f;
	particle_->SetParticleParam(param_);
	particle_->Update();


#ifdef _DEBUG

#endif
}

void PlayerParticle::Draw(const Camera& camera)
{
	particle_->Draw(camera);
}
