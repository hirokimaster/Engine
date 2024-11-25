/**
* @file ExplosionParticle.cpp
* @brief 爆発のパーティクル
* @author 仁平 琉乃
*/

#include "ExplosionParticle.h"

void ExplosionParticle::Initialize()
{
	// 爆発の中心のパーティクル
	texHandle_ = TextureManager::Load("resources/Player/smoke.png");
	particle_ = std::make_unique<GPUParticle>();
	particle_->SetModel("Player/plane.obj");
	particle_->Initialize();
	particle_->SetTexHandle(texHandle_);

	// particleの値
	param_.count = 300;
	param_.frequency = 0.1f;
	param_.frequencyTime = 0.0f;
	param_.radius = 1.0f;
	param_.emit = 1;
	param_.rangeTranslate.min = Vector3(0.0f, 0.0f, 0.0f);
	param_.rangeTranslate.max = Vector3(0.0f, 0.0f, 0.0f);
	param_.rangeScale.min = Vector3(0.3f, 0.3f, 0.3f);
	param_.rangeScale.max = Vector3(0.5f, 0.5f, 0.5f);
	param_.rangeLifeTime.min = 1.0f;
	param_.rangeLifeTime.max = 1.0f;
	param_.rangeVelocity.min = Vector3(-0.01f, -0.01f, -0.01f);
	param_.rangeVelocity.max = Vector3(0.01f, 0.01f, 0.01f);
	param_.rangeCurrentTime.min = 0.0f;
	param_.rangeCurrentTime.max = 5.0f;
	param_.rangeColor.min = Vector3(1.0f, 1.0f, 0.0f);
	param_.rangeColor.max = Vector3(5.0f, 5.0f, 0.0f);
	param_.rangeAlpha.min = 0.0f;
	param_.rangeAlpha.max = 1.0f;

	particle_->SetParticleParam(param_);

	// 爆破後に広がるパーティクル
	texHandleSmoke_ = TextureManager::Load("resources/TempTexture/circle2.png");
	particleSmoke_ = std::make_unique<GPUParticle>();
	particleSmoke_->SetModel("Player/plane.obj");
	particleSmoke_->Initialize();
	particleSmoke_->SetTexHandle(texHandleSmoke_);

	// particleの値
	paramSmoke_.count = 300;
	paramSmoke_.frequency = 0.3f;
	paramSmoke_.frequencyTime = 0.0f;
	paramSmoke_.radius = 1.0f;
	paramSmoke_.emit = 1;
	paramSmoke_.rangeTranslate.min = Vector3(-1.0f, -1.0f, -1.0f);
	paramSmoke_.rangeTranslate.max = Vector3(1.0f, 1.0f, 1.0f);
	paramSmoke_.rangeScale.min = Vector3(1.0f, 1.0f, 1.0f);
	paramSmoke_.rangeScale.max = Vector3(2.0f, 2.0f, 2.0f);
	paramSmoke_.rangeLifeTime.min = 1.0f;
	paramSmoke_.rangeLifeTime.max = 5.0f;
	paramSmoke_.rangeVelocity.min = Vector3(-0.03f, -0.03f, -0.03f);
	paramSmoke_.rangeVelocity.max = Vector3(0.03f, 0.03f, 0.03f);
	paramSmoke_.rangeCurrentTime.min = 0.0f;
	paramSmoke_.rangeCurrentTime.max = 5.0f;
	paramSmoke_.rangeColor.min = Vector3(0.4f, 0.4f, 0.4f);
	paramSmoke_.rangeColor.max = Vector3(0.6f, 0.6f, 0.6f);
	paramSmoke_.rangeAlpha.min = 0.6f;
	paramSmoke_.rangeAlpha.max = 0.8f;

	particleSmoke_->SetParticleParam(paramSmoke_);
	Vector4 color = { 0.2f,0.2f,0.2f,0.2f };
	materialSmoke_.color = color;
	particleSmoke_->SetColor(materialSmoke_);

}

void ExplosionParticle::Update()
{
	paramSmoke_.translate = position_;
	paramSmoke_.frequencyTime += 1.0f / 60.0f;
	particleSmoke_->SetParticleParam(paramSmoke_);
	particleSmoke_->Update();

	if (isExplosion_) {
		particle_.reset();
	}
	else {
		param_.translate = position_;
		param_.frequencyTime += 1.0f / 60.0f;
		param_.rangeColor.min.y *= 0.5f;
		param_.rangeColor.max.y *= 0.5f;
		particle_->SetParticleParam(param_);
		particle_->Update();
	}
}

void ExplosionParticle::Draw(const Camera& camera)
{
	if (!isExplosion_) {
		particle_->Draw(camera);
	}
	
	particleSmoke_->Draw(camera);
}
