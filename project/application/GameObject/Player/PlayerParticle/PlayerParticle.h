/**
* @file PlayerParticle.h
* @brief プレイヤー（自機）のパーティクル
* @author 仁平 琉乃
*/

#pragma once
#include "engine/ParticleSystem/GPUParticle.h"
#include "engine/TextureManager/TextureManager.h"

class PlayerParticle {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const Camera& camera);

#pragma region setter
	// setter
	void SetPosition(const Vector3& position) { position_ = position; }

#pragma endregion

private:
	std::unique_ptr<GPUParticle> particle_ = {};
	uint32_t texHandle_ = 0;
	Vector3 position_ = {};
	EmitterSphere param_{};
};
