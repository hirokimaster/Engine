/**
* @file ExplosionParticle.cpp
* @brief 爆発のパーティクル
* @author 仁平 琉乃
*/

#include "ExplosionParticle.h"

void ExplosionParticle::Initialize()
{
	texHandle_ = TextureManager::Load("resources/Player/smoke.png");
	particle_ = std::make_unique<ParticleSystem>();
	randomEngine_ = particle_->random();
	particle_->Initialize();
	particle_->SetModel("Player/plane.obj");
	particle_->SetTexHandle(texHandle_);
	distributionParam_ = {
	{ -2.0f, 2.0f }, // velocity
	{ 1.0f, 5.0f },   // color
	{ 0.7f, 1.0f },   // rotate
	{ 0.5f, 1.0f },   // scale
	{ 0.3f, 0.6f }	  // lifeTime
	};
	particle_->SetDistributionParam(distributionParam_);

	emit_.count = 2;
	emit_.frequency = 0.01f;
	emit_.frequencyTime = 0.0f;

}

void ExplosionParticle::Update()
{
	particle_->SetPosition(position_);

	emit_.frequencyTime += deltaTime_;
	if (emit_.frequency <= emit_.frequencyTime) {
		particles_.splice(particles_.end(), particle_->Emission(emit_, randomEngine_));
		emit_.frequencyTime -= emit_.frequency;
	}


	for (std::list<Particle>::iterator particleItr = particles_.begin();
		particleItr != particles_.end(); ++particleItr) {
		(*particleItr).color.y *= 0.5f;
		(*particleItr).color.z = 0.0f;
	}

	for (std::list<Particle>::iterator particleItr = particles_.begin();
		particleItr != particles_.end(); ++particleItr) {

		(*particleItr).worldTransform.translate = Add((*particleItr).worldTransform.translate, Multiply(deltaTime_, (*particleItr).velocity));
		(*particleItr).worldTransform.UpdateMatrix();

	}
}

void ExplosionParticle::Draw(const Camera& camera)
{
	particle_->Draw(particles_, camera);
}
