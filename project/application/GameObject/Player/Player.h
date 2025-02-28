/**
* @file Player.h
* @brief プレイヤー（自機）の生成、挙動
* @author 仁平 琉乃
*/

#pragma once
#include "application/GameObject/Player/Bullet/PlayerBullet.h"
#include "engine/Graphics/TextureManager/TextureManager.h"
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "engine/2d/Sprite/Sprite.h"
#include <vector>
#include "engine/3d/Object3DPlacer/Object3DPlacer.h"
#include "application/GameObject/Rail/Rail.h"
#include "application/AdjustmentVariables/AdjustmentVariables.h"

class LockOn;

class Player
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
	/// 弾の更新処理
	/// </summary>
	void UpdateBullet();

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

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() const { return bullets_; }

	Vector3 GetWorldPosition()const;

	Vector2 GetScreenPosition2DReticle() { return screenPositionReticle_; }

	Vector3 GetRotate()const { return object_->GetWorldTransform().rotate; }

	bool GetIsHitEnemyFire() { return isHitEnemyFire_; }

	bool GetIsDead() { return isDead_; }

	uint32_t GetHp() { return hp_; }

	float GetDeadTimer() { return deadTimer_; }

	uint32_t GetDestroyCount() { return destroyCount_; }

	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return object_->GetWorldTransform(); }

	Collider* GetCollider() { return collider_.get(); }

#pragma endregion

#pragma region setter

	void SetParent(const WorldTransform* parent) { object_->SetParent(parent); }

	void SetPosition(const Vector3& position) { object_->SetPosition(position); }

	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

	//void SetPositionZ(float positionZ) { object_->SetPosition({ 0,0,positionZ }); }

	void SetHp(uint32_t hp) { hp_ = hp; }

#pragma endregion

private:
	float moveSpeed_; // 移動スピード
	float bulletSpeed_;
	std::list<std::unique_ptr<PlayerBullet>> bullets_; // 弾のリスト
	LockOn* lockOn_ = nullptr; // ロックオンのポインタ
	Vector2 screenPositionReticle_{};
	std::unique_ptr<Object3DPlacer> object_ = nullptr;
	bool isDead_ = false; // 死んだか
	bool isHitEnemyFire_ = false; // 敵の攻撃が当たったか
	int32_t hp_ = 1;
	float deadTimer_ = 60.0f;
	uint32_t destroyCount_ = 0; // 敵を倒した数
	Vector3 moveMinLimit_ = { -30.0f, 10.0f, 0.0f }; // 移動最小値
	Vector3 moveMaxLimit_ = { 30.0f, 50.0f, 0.0f };    // 移動最大値
	std::unique_ptr<Collider> collider_ = nullptr;
	// UI
	std::unique_ptr<Sprite> spriteAttack_ = nullptr;
	std::unique_ptr<Sprite> spriteMove_ = nullptr;
};
