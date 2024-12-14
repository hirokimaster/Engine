/**
* @file Enemy.h
* @brief 敵の生成、挙動
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "application/GameObject/Enemy/Bullet/EnemyBullet.h"
#include "engine/TextureManager/TextureManager.h"
#include "engine/ParticleSystem/ParticleSystem.h"
#include "engine/Object3DPlacer/Object3DPlacer.h"
#include "application/GameObject/Enemy/PhaseState/EnemyStateSortie.h"
#include "application/GameObject/Enemy/PhaseState/EnemyStateFire.h"
#include "application/GameObject/Player/PlayerParticle/ExplosionParticle.h"
#include "engine/ParticleManager/ParticleManager.h"

class Player;

class Enemy : public Collider {
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
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 状態を変える
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(std::unique_ptr<IPhaseStateEnemy> newState);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Fire();

private:

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision()override;

	/// <summary>
	/// 弾の更新
	/// </summary>
	void BulletUpdate();

	/// <summary>
	/// 調整項目の追加
	/// </summary>
	void AddAdjustmentVariables();

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyAdjustmentVariables();

public:

#pragma region getter

	Vector3 GetWorldPosition() const override;

	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() const { return bullets_; }

	bool GetIsDead()const { return isDead_; }

	Vector3 GetStartPosition() { return startPosition_; }

	Vector3 GetEndPosition() { return endPosition_; }

	bool GetIsSortie() { return isSortie_; }

	uint32_t GetID() const { return id_; }

#pragma endregion

#pragma region setter

	void SetPosition(Vector3 position) { worldTransform_.translate = position; }

	void SetPlayer(Player* player) { player_ = player; }

	void SetVelocity(Vector3 velocity) { velocity_ = velocity; }

	void SetStartPosition(Vector3 start) { startPosition_ = start; }

	void SetEndPosition(Vector3 end) { endPosition_ = end; }

	void SetIsSortie(bool isSortie) { isSortie_ = isSortie; }

	void SetIsDead(bool isDead) { isDead_ = isDead; }

	void SetID(int id) { id_ = id; }

#pragma endregion

private:
	uint32_t id_ = 0; // enemyの識別番号
	bool isDead_ = false; // デスフラグ
	std::list<std::unique_ptr<EnemyBullet>> bullets_; // 弾のリスト
	uint32_t texHandleBullet_ = 0; // bulletのtexHandle
	WorldTransform worldTransform_{};
	std::unique_ptr<Object3DPlacer> object_ = nullptr;
	static const int32_t kLifeTime_ = 60 * 50; // 生きてる時間
	int32_t deathTimer_ = kLifeTime_; // デスタイマー
	Player* player_ = nullptr;
	std::unique_ptr<IPhaseStateEnemy> phaseState_; // 行動フェーズ（状態）
	Vector3 velocity_{}; // 移動ベクトル
	Vector3 startPosition_{}; // スポーンする座標
	Vector3 endPosition_{}; // でできた後の最終座標
	bool isSortie_ = false;
	bool isParticle_ = false; 
	uint32_t particleTimer_ = 120;
	float bulletSpeed_ = 0.2f;
	ParticleManager* particleManager_ = nullptr;
	uint32_t texHandleExplosion_ = 0;
	uint32_t texHandleSmoke_ = 0;
};

