/**
* @file ParticleManager.cpp
* @brief particleの一括管理
* @author 仁平 琉乃
*/

#include "ParticleManager.h"
#include <filesystem> // 追加

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Initialize()
{
	// 空にしておく
	particles_.clear();
	while (!pool_.empty()) pool_.pop();

	ModelManager::GetInstance()->LoadObjModel("Player/plane.obj");
	// パラメーター読み込み
	ParticleEditor::GetInstance()->LoadFiles();

	// あらかじめインスタンスを作っておく
	for (uint32_t i = 0; i < kPoolSize; ++i) {
		Create();
	}

	// ParticleParamディレクトリ内の全jsonファイルを自動で読み込む
	std::filesystem::path dir("resources/ParticleParam/");
	if (std::filesystem::exists(dir)) {
		for (const auto& entry : std::filesystem::directory_iterator(dir)) {
			if (entry.is_regular_file() && entry.path().extension() == ".json") {
				std::string name = entry.path().stem().string();
				CreateParam(name.c_str());
				ApplyParam(name.c_str());
			}
		}
	}
}

void ParticleManager::CreateParam(const char* particleName)
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

void ParticleManager::ApplyParam(const char* particleName)
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
}

void ParticleManager::Create()
{
	// 最大を越えないようにする
	if (kPoolSize > particles_.size()) {
		unique_ptr<GPUParticle> particle = make_unique<GPUParticle>();
		particle->SetModel("Player/plane.obj");
		particle->Initialize();

		// キューに入れる
		GPUParticle* ptr = particle.get();
		particles_.push_back(std::move(particle));
		pool_.push(ptr);
	}
}
	

void ParticleManager::Push(GPUParticle* particle)
{
	// particleがnullだったら通っちゃだめ
	if (!particle) return;

	// アクティブ状態解除
	particle->SetIsActive(false);
	// 生存にする
	particle->SetIsDead(false);
	// 生存時間をリセットする
	particle->ResetLifeTime();
	// キューに戻す
	pool_.push(particle);
}

void ParticleManager::Update()
{
	
	for (auto& particle : particles_) {
		if (particle->GetIsActive()) {
			
			particle->Update();
		}

		// パーティクルの生存時間を過ぎたら
		if (particle->GetIsDead()) {
			// キューに戻す
			Push(particle.get());
		}
	}
}

void ParticleManager::UpdateEditor()
{
	ParticleEditor::GetInstance()->Update();
}

void ParticleManager::Draw(const Camera& camera)
{
	for (auto& particle : particles_) {
		if (particle->GetIsActive()) {
			particle->Draw(camera);
		}
	}
}

GPUParticle* ParticleManager::GetParticle(const string& name, const string& texture)
{
	// 空のときは新しく作る
	if (pool_.empty()) {
		// 最大数を超えてたら
		if (particles_.size() >= kPoolSize) {
			return nullptr;
		}
		Create();
	}

	// 取り出す
	GPUParticle* ptr = pool_.front();
	pool_.pop();
	ptr->SetTexHandle(TextureManager::GetTexHandle(texture)); // textureを決める(初期は白のtexture)
	ptr->SetParticleParam(params_[name]); // ここで指定したパラメーターを入れる
	return 	ptr;
}
