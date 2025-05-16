/**
* @file EngineParticle.cpp
* @brief 自機のエンジンのパーティクル
* @author 仁平 琉乃
*/

#include "EngineParticle.h"

void EngineParticle::Initialize()
{
	// texture
	TextureManager::Load("resources/Player/engine.png");
	particleManager_ = ParticleManager::GetInstance();
	// particleManagerから取ってくる
	rightEngine_ = particleManager_->GetParticle("engine_right", "Player/engine.png");
	leftEngine_ = particleManager_->GetParticle("engine_left", "Player/engine.png");
	// 生存時間設定
	lifeTime_ = 60000.0f;
	rightEngine_->SetLifeTime(lifeTime_);
	leftEngine_->SetLifeTime(lifeTime_);
}

void EngineParticle::Update(const Vector3& playerRotate, const Vector3& playerPosition)
{
	// パーティクルのパラメーターを設定
	leftEngine_->SetParticleParam(particleManager_->GetParam("engine_left"));
	rightEngine_->SetParticleParam(particleManager_->GetParam("engine_right"));
	// アクティブにする
	leftEngine_->SetIsActive(true);
	rightEngine_->SetIsActive(true);
	// 回転行列計算
	Matrix4x4 rotMat = MakeRotateMatrix(playerRotate);
	// 回転を考慮してoffsetを決める
	Vector3 rotatedOffsetL = Transform(particleOffsetL_, rotMat);
	Vector3 rotatedOffsetR = Transform(particleOffsetR_, rotMat);
	leftEngine_->SetPosition(playerPosition + rotatedOffsetL);
	rightEngine_->SetPosition(playerPosition + rotatedOffsetR);
}
