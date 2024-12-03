#include "ParticleManager.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Initialize()
{
	editor_ = make_unique<ParticleEditor>();
	editor_->LoadFiles();
}

void ParticleManager::StartEditor(const string& particleName)
{
	// particleの値を設定する
	// エディター起動
	EmitterSphere emitter{};
	editor_->CreateParticle(particleName);
	editor_->AddParam(particleName, "translate", emitter.translate);
	editor_->AddParam(particleName, "radius", emitter.radius);
	editor_->AddParam(particleName, "count", emitter.count);
	editor_->AddParam(particleName, "frequency", emitter.frequency);
	editor_->AddParam(particleName, "frequencyTime", emitter.frequencyTime);
	editor_->AddParam(particleName, "emit", emitter.emit);
	editor_->AddParam(particleName, "rangeTranslate", emitter.rangeTranslate);
	editor_->AddParam(particleName, "rangeScale", emitter.rangeScale);
	editor_->AddParam(particleName, "rangeLifeTime", emitter.rangeLifeTime);
	editor_->AddParam(particleName, "rangeVelocity", emitter.rangeVelocity);
	editor_->AddParam(particleName, "rangeCurrentTime", emitter.rangeCurrentTime);
	editor_->AddParam(particleName, "rangeColor", emitter.rangeColor);
	editor_->AddParam(particleName, "rangeAlpha", emitter.rangeAlpha);
}

void ParticleManager::ApplyParticleInfo(const char* particleName)
{
	editor_->GetValue<Vector3>(particleName, "translate");
	editor_->GetValue<float>(particleName, "radius");
	editor_->GetValue<int32_t>(particleName, "count");
	editor_->GetValue<float>(particleName, "frequency");
	editor_->GetValue<float>(particleName, "frequencyTime");
	editor_->GetValue<int32_t>(particleName, "emit");
	editor_->GetValue<ParticleRange3d>(particleName, "rangeTranslate");
	editor_->GetValue<ParticleRange3d>(particleName, "rangeScale");
    editor_->GetValue<ParticleRange1d>(particleName, "rangeLifeTime");
	editor_->GetValue<ParticleRange3d>(particleName, "rangeVelocity");
	editor_->GetValue<ParticleRange1d>(particleName, "rangeCurrentTime");
	editor_->GetValue<ParticleRange3d>(particleName, "rangeColor");
	editor_->GetValue<ParticleRange1d>(particleName, "rangeAlpha");
}

void ParticleManager::CreateParticle(const string& particleName)
{

}

void ParticleManager::Update()
{
	editor_->Update();
}
