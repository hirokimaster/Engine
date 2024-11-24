/**
* @file ExplosionParticle.cpp
* @brief 爆発のパーティクル
* @author 仁平 琉乃
*/

#include "ExplosionParticle.h"

void ExplosionParticle::Initialize()
{
	texHandle_ = TextureManager::Load("resources/Player/smoke.png");
	particle_ = std::make_unique<GPUParticle>();
	particle_->SetModel("Player/plane.obj");
	particle_->Initialize();
	particle_->SetTexHandle(texHandle_);

	// particleの値
	param_.count = 500;
	param_.frequency = 1.0f;
	param_.frequencyTime = 1.0f;
	param_.radius = 1.0f;
	param_.emit = 1;
	param_.rangeTranslate.min = Vector3(0.0f, 0.0f, 0.0f);
	param_.rangeTranslate.max = Vector3(0.0f, 0.0f, 0.0f);
	param_.rangeScale.min = Vector3(0.5f, 0.5f, 0.5f);
	param_.rangeScale.max = Vector3(1.0f, 1.0f, 1.0f);
	param_.rangeLifeTime.min = 1.0f;
	param_.rangeLifeTime.max = 1.0f;
	param_.rangeVelocity.min = Vector3(-0.05f, -0.05f, -0.05f);
	param_.rangeVelocity.max = Vector3(0.05f, 0.05f, 0.05f);
	param_.rangeCurrentTime.min = 0.0f;
	param_.rangeCurrentTime.max = 5.0f;
	param_.rangeColor.min = Vector3(1.0f, 1.0f, 0.0f);
	param_.rangeColor.max = Vector3(5.0f, 5.0f, 0.0f);
	param_.rangeAlpha.min = 0.0f;
	param_.rangeAlpha.max = 1.0f;

	particle_->SetParticleParam(param_);

}

void ExplosionParticle::Update()
{
	param_.translate = position_;
	param_.frequencyTime += 1.0f / 60.0f;
	param_.rangeColor.min.y *= 0.5f;
	param_.rangeColor.max.y *= 0.5f;
	particle_->SetParticleParam(param_);
	particle_->Update();
}

void ExplosionParticle::Draw(const Camera& camera)
{
	particle_->Draw(camera);
}
