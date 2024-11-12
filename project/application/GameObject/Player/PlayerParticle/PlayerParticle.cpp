/**
* @file PlayerParticle.cpp
* @brief プレイヤー（自機）のパーティクル
* @author 仁平 琉乃
*/

#include "PlayerParticle.h"

void PlayerParticle::Initialize()
{
	texHandle_ = TextureManager::Load("resources/Player/smoke.png");
	particle_ = std::make_unique<ParticleSystem>();
	randomEngine_ = particle_->random();
	particle_->Initialize();
	particle_->SetModel("Player/plane.obj");
	particle_->SetTexHandle(texHandle_);
	distributionParam_ = { 
	{ -4.0f, -1.0f }, // velocity
	{ 0.0f, 1.0f },   // color
	{ 0.0f, 1.0f },   // rotate
	{ 0.2f, 0.5f },   // scale
	{ 0.5f, 1.0f }	  // lifeTime
	};
	particle_->SetDistributionParam(distributionParam_);

	emit_.count = 2;
	emit_.frequency = 0.01f;
	emit_.frequencyTime = 0.0f;
}

void PlayerParticle::Update()
{
	particle_->SetPosition(position_);
	accelerationField.area.min = areaMin_;
	accelerationField.area.max = areaMax_;

	emit_.frequencyTime += deltaTime_;
	if (emit_.frequency <= emit_.frequencyTime) {
		particles_.splice(particles_.end(), particle_->Emission(emit_, randomEngine_));
		emit_.frequencyTime -= emit_.frequency;
	}

	accelerationField.acceleration = velocity_;

	for (std::list<Particle>::iterator particleItr = particles_.begin();
		particleItr != particles_.end(); ++particleItr) {
		(*particleItr).velocity.x = velocity_.x;
		(*particleItr).velocity.y = velocity_.y;
		(*particleItr).color.x = 1.0f;
		(*particleItr).color.y = 1.0f;
		(*particleItr).color.z = 1.0f;
	}

	for (std::list<Particle>::iterator particleItr = particles_.begin();
		particleItr != particles_.end(); ++particleItr) {

		if (particle_->IsCollision(accelerationField.area, (*particleItr).worldTransform.translate)) {
			(*particleItr).velocity.x = (*particleItr).velocity.x + deltaTime_ * accelerationField.acceleration.x;
			(*particleItr).velocity.y = (*particleItr).velocity.y + deltaTime_ * accelerationField.acceleration.y;
		}

		(*particleItr).worldTransform.translate = Add((*particleItr).worldTransform.translate, Multiply(deltaTime_, (*particleItr).velocity));
		(*particleItr).worldTransform.UpdateMatrix();

	}

#ifdef _DEBUG

	particle_->SetDistributionParam(distributionParam_);

	ImGui::Begin("param");

	ImGui::DragFloat2("velo", &distributionParam_.velocity.x, 0.01f, -100.0f, 100.0f);
	ImGui::DragFloat2("color", &distributionParam_.color.x, 0.01f, 0.0f, 5.0f);
	ImGui::DragFloat2("rotate", &distributionParam_.rotate.x, 0.01f, -100.0f, 100.0f);
	ImGui::DragFloat2("scale", &distributionParam_.scale.x, 0.01f, -100.0f, 100.0f);
	ImGui::DragFloat2("lifeTime", &distributionParam_.lifeTime.x, 0.01f, 0.0f, 100.0f);
	ImGui::DragFloat("frequency", &emit_.frequency, 0.01f, -100.0f, 100.0f);
	ImGui::DragFloat("frequencyTime", &emit_.frequencyTime, 0.01f, -100.0f, 100.0f);

	ImGui::End();

#endif
}

void PlayerParticle::Draw(const Camera& camera)
{
	particle_->Draw(particles_, camera);
}
