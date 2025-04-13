/**
* @file Player.h
* @brief プレイヤー（自機）の生成、挙動
* @author 仁平 琉乃
*/

#pragma once
#include "application/GameObject/Bullet/PlayerBullet/PlayerBullet.h"
#include "engine/Graphics/TextureManager/TextureManager.h"
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "engine/2d/Sprite/Sprite.h"
#include <vector>
#include "application/AdjustmentVariables/AdjustmentVariables.h"
#include "engine/3d/Object3DPlacer/BaseObject.h"
#include "application/GameObject/Bullet/BulletObjectPool/BulletObjectPool.h"
#include "engine/3d/PlaneProjectionShadow/PlaneProjectionShadow.h"

class LockOn;

class Player : BaseObject
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

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
	/// UIの描画
	/// </summary>
	/// <param name="camera"></param>
	void DrawUI();

	/// <summary>
	/// 撃破数を足す
	/// </summary>
	void AddDestroyCount() { ++destroyCount_; }

private: // クラス内でしか使わない

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision();

	/// <summary>
	/// 自機の回転
	/// </summary>
	void Rotate();

	/// <summary>
	/// ダメージ食らう
	/// </summary>
	void IncurDamage();

	/// <summary>
	/// デバック描画(ImGui)
	/// </summary>
	void DebugDraw();

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

	Vector2 GetScreenPosition2DReticle()const { return screenPositionReticle_; }

	Vector3 GetRotate()const { return object_->GetWorldTransform().rotate; }

	bool GetIsHitEnemyFire()const { return isHitEnemyFire_; }

	bool GetIsDead()const { return isDead_; }

	uint32_t GetHp()const { return hp_; }

	float GetDeadTimer()const { return deadTimer_; }

	uint32_t GetDestroyCount()const{ return destroyCount_; }

	const WorldTransform& GetWorldTransform()const { return object_->GetWorldTransform(); }

	Vector3 GetWorldPosition()const;

	Collider* GetCollider() { return collider_.get(); }

#pragma endregion

#pragma region setter

	void SetParent(const WorldTransform* parent) { object_->SetParent(parent); }

	void SetPosition(const Vector3& position) { object_->SetPosition(position); }

	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

	void SetHp(uint32_t hp) { hp_ = hp; }

	void SetBulletObjectPool(BulletObjectPool* ptr) { bulletObjectPool_ = ptr; }

#pragma endregion

private:
	float moveSpeed_; // 移動スピード
	float bulletSpeed_; // 弾のスピード
	LockOn* lockOn_ = nullptr; // ロックオンのポインタ
	Vector2 screenPositionReticle_{};
	bool isDead_ = false; // 死んだか
	bool isHitEnemyFire_ = false; // 敵の攻撃が当たったか
	int32_t hp_ = 1; // ヒットポイント
	float deadTimer_ = 60.0f; // 死んだときのタイマー
	uint32_t destroyCount_ = 0; // 敵を倒した数
	Vector3 moveMinLimit_ = { -30.0f, 10.0f, 0.0f }; // 移動最小値
	Vector3 moveMaxLimit_ = { 30.0f, 50.0f, 0.0f };    // 移動最大値
	// UI
	std::unique_ptr<Sprite> spriteAttack_;
	std::unique_ptr<Sprite> spriteMove_;
	float attackTimer_ = 0.0f; // 弾の連射速度用のタイマー
	Vector3 rotate_{}; // 回転の初期値
	float rotateSpeed_; // 回転速度
	float rotateLerpFactor_; // Lerpの強さ
	BulletObjectPool* bulletObjectPool_ = nullptr; // ポインタ借りる
	ParticleManager* particleManager_ = nullptr;
	std::unique_ptr<PlaneProjectionShadow> shadow_; // 影
};
