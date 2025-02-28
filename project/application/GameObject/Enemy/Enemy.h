/**
* @file Enemy.h
* @brief 敵の生成、挙動
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "application/GameObject/Enemy/Bullet/EnemyBullet.h"
#include "engine/Graphics/TextureManager/TextureManager.h"
#include "engine/3d/Object3DPlacer/Object3DPlacer.h"
#include "application/GameObject/Enemy/PhaseState/EnemyStateSortie.h"
#include "application/GameObject/Enemy/PhaseState/EnemyStateFire.h"

class Player;

class Enemy{
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
	void Draw(const Camera& camera);

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 状態を変える
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(std::unique_ptr<BasePhaseStateEnemy> newState);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Fire();

private:

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision();

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

	Vector3 GetWorldPosition() const;

	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() const { return bullets_; }

	bool GetIsDead()const { return isDead_;}

	bool GetIsSortie() { return isSortie_; }

	Collider* GetCollider() { return collider_.get(); }

#pragma endregion

#pragma region setter

	void SetPosition(const Vector3& position) { object_->SetPosition(position); }

	void SetPlayer(Player* player) { player_ = player; }

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

	void SetIsSortie(bool isSortie) { isSortie_ = isSortie; }

	void SetIsDead(bool isDead) { isDead_ = isDead; }

	void SetMoveControlPoints(const Vector3& controlPoints) { moveControlPoints_.push_back(controlPoints); }

#pragma endregion

private:
	bool isDead_ = false; // デスフラグ
	std::list<std::unique_ptr<EnemyBullet>> bullets_; // 弾のリスト
	std::unique_ptr<Object3DPlacer> object_ = nullptr;
	static const int32_t kLifeTime_ = 60 * 50; // 生きてる時間
	int32_t deathTimer_ = kLifeTime_; // デスタイマー
	Player* player_ = nullptr; // playerのポインタ
	std::unique_ptr<BasePhaseStateEnemy> phaseState_; // 行動フェーズ（状態）
	Vector3 velocity_{}; // 移動ベクトル
	std::vector<Vector3> moveControlPoints_{}; // 移動ルートの制御点を入れとくコンテナ
	bool isSortie_ = false; // 出撃してるか
	float bulletSpeed_ = 0.2f; // 弾のスピード
	std::unique_ptr<Collider> collider_ = nullptr; // collider
};

