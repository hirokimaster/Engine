/**
* @file BossEnemy.h
* @brief ボス敵の生成、挙動
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "engine/TextureManager/TextureManager.h"
#include "engine/ParticleSystem/ParticleSystem.h"
#include "engine/Object3DPlacer/Object3DPlacer.h"
#include "application/GameObject/Player/Player.h"

// 振る舞い
enum class Behavior {
	kRoot
};

class BossEnemy : public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="object"></param>
	/// <param name="texHandle"></param>
	/// <param name="model"></param>
	void Initialize(uint32_t texHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(Camera& camera);

	/// <summary>
	/// 衝突した時に返ってくる場所
	/// </summary>
	void OnCollision()override;

#pragma region setter

	void SetPlayer(Player* player) { player_ = player; }

#pragma endregion

#pragma region getter

	Vector3 GetWorldPosition()const override;

	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() const { return bullets_; }

#pragma endregion

private: // クラス内でしか使わない関数

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 通常攻撃
	/// </summary>
	void UsuallyFire();

	/// <summary>
	/// 弾の更新処理
	/// </summary>
	void UpdateBullet();

#pragma region behavior

	/// <summary>
	/// 通常行動初期化
	/// </summary>
	void BehaviorRootInitialize();
	/// <summary>
	/// 通常行動更新
	/// </summary>
	void BehaviorRootUpdate();

#pragma endregion

private: // メンバ変数
	WorldTransform worldTransform_{};
	Vector3 offset_{};
	std::unique_ptr<Object3DPlacer> object_ = nullptr;
	Behavior behavior_;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	Player* player_ = nullptr;
	// bullet
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	uint32_t texHandleBullet_ = 0;
	uint32_t fireTimer_ = 0;
	uint32_t fireInterval_ = 0;

};