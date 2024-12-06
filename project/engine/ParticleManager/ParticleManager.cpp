#include "ParticleManager.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Initialize()
{
	// 読み込み
	ParticleEditor::GetInstance()->LoadFiles();
}

void ParticleManager::StartEditor(const char* particleName)
{
	// particleの値を設定する
	// エディター起動
	ParticleEditor* editor = ParticleEditor::GetInstance();
	editor->CreateParticle(particleName);
	editor->AddParam(particleName, "translate", params_[particleName].translate);
	editor->AddParam(particleName, "radius", params_[particleName].radius);
	editor->AddParam(particleName, "count", static_cast<int32_t>(params_[particleName].count));
	editor->AddParam(particleName, "frequency", params_[particleName].frequency);
	editor->AddParam(particleName, "frequencyTime", params_[particleName].frequencyTime);
	editor->AddParam(particleName, "emit", static_cast<int32_t>(params_[particleName].emit));
	// rangeTranslateを設定
	editor->AddParam(particleName, "rangeTranslate_min",params_[particleName].rangeTranslate.min);
	editor->AddParam(particleName, "rangeTranslate_max", params_[particleName].rangeTranslate.max);
	// rangeScaleを設定
	editor->AddParam(particleName, "rangeScale_min", params_[particleName].rangeScale.min);
	editor->AddParam(particleName, "rangeScale_max", params_[particleName].rangeScale.max);
	// rangeLifeTimeを設定
	editor->AddParam(particleName, "rangeLifeTime_min", params_[particleName].rangeLifeTime.min);
	editor->AddParam(particleName, "rangeLifeTime_max", params_[particleName].rangeLifeTime.max);
	// rangeVelocityを設定
	editor->AddParam(particleName, "rangeVelocity_min", params_[particleName].rangeVelocity.min);
	editor->AddParam(particleName, "rangeVelocity_max", params_[particleName].rangeVelocity.max);
	// rangeCurrentTimeを設定
	editor->AddParam(particleName, "rangeCurrentTime_min", params_[particleName].rangeCurrentTime.min);
	editor->AddParam(particleName, "rangeCurrentTime_max", params_[particleName].rangeCurrentTime.max);
	// rangeColorを設定
	editor->AddParam(particleName, "rangeColor_min", params_[particleName].rangeColor.min);
	editor->AddParam(particleName, "rangeColor_max", params_[particleName].rangeColor.max);
	// rangeAlphaを設定
	editor->AddParam(particleName, "rangeAlpha_min", params_[particleName].rangeAlpha.min);
	editor->AddParam(particleName, "rangeAlpha_max", params_[particleName].rangeAlpha.max);
}

void ParticleManager::ApplyParticleInfo(const char* particleName)
{
	ParticleEditor* editor = ParticleEditor::GetInstance();
	// params_のparticleNameに対応するemitterを更新を適用する
	params_[particleName].translate = editor->GetValue<Vector3>(particleName, "translate");
	params_[particleName].radius = editor->GetValue<float>(particleName, "radius");
	params_[particleName].count = static_cast<uint32_t>(editor->GetValue<int32_t>(particleName, "count"));
	params_[particleName].frequency = editor->GetValue<float>(particleName, "frequency");
	params_[particleName].frequencyTime = editor->GetValue<float>(particleName, "frequencyTime");
	params_[particleName].emit = static_cast<uint32_t>(editor->GetValue<int32_t>(particleName, "emit"));
	// rangeTranslateのminとmaxを設定
	params_[particleName].rangeTranslate.min = editor->GetValue<Vector3>(particleName, "rangeTranslate_min");
	params_[particleName].rangeTranslate.max = editor->GetValue<Vector3>(particleName, "rangeTranslate_max");
	// rangeScaleのminとmaxを設定
	params_[particleName].rangeScale.min = editor->GetValue<Vector3>(particleName, "rangeScale_min");
	params_[particleName].rangeScale.max = editor->GetValue<Vector3>(particleName, "rangeScale_max");
	// rangeLifeTimeのminとmaxを設定
	params_[particleName].rangeLifeTime.min = editor->GetValue<float>(particleName, "rangeLifeTime_min");
	params_[particleName].rangeLifeTime.max = editor->GetValue<float>(particleName, "rangeLifeTime_max");
	// rangeVelocityのminとmaxを設定
	params_[particleName].rangeVelocity.min = editor->GetValue<Vector3>(particleName, "rangeVelocity_min");
	params_[particleName].rangeVelocity.max = editor->GetValue<Vector3>(particleName, "rangeVelocity_max");
	// rangeCurrentTimeのminとmaxを設定
	params_[particleName].rangeCurrentTime.min = editor->GetValue<float>(particleName, "rangeCurrentTime_min");
	params_[particleName].rangeCurrentTime.max = editor->GetValue<float>(particleName, "rangeCurrentTime_max");
	// rangeColorのminとmaxを設定
	params_[particleName].rangeColor.min = editor->GetValue<Vector3>(particleName, "rangeColor_min");
	params_[particleName].rangeColor.max = editor->GetValue<Vector3>(particleName, "rangeColor_max");
	// rangeAlphaのminとmaxを設定
	params_[particleName].rangeAlpha.min = editor->GetValue<float>(particleName, "rangeAlpha_min");
	params_[particleName].rangeAlpha.max = editor->GetValue<float>(particleName, "rangeAlpha_max");

	particles_[particleName]->SetParticleParam(params_[particleName]);
}

void ParticleManager::CreateParticle(const string& particleName, const string& model, uint32_t texHandle)
{
	// 読み込まれてたら抜ける
	if (particles_.contains(particleName)) {
		return;
	}
	unique_ptr<GPUParticle> particle = make_unique<GPUParticle>();
	particle->SetModel(model);
	particle->Initialize();
	particle->SetTexHandle(texHandle);
	particle->SetParticleParam(params_[particleName]);

	particles_.insert(pair(particleName, move(particle)));
}

void ParticleManager::Update(const string& particleName)
{
	particles_[particleName]->Update();
}

void ParticleManager::UpdateEditor()
{
	ParticleEditor::GetInstance()->Update();
}

void ParticleManager::Draw(const string& particleName, const Camera& camera)
{
	particles_[particleName]->Draw(camera);
}
