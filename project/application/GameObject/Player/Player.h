/**
* @file Player.h
* @brief プレイヤー（自機）の生成、挙動
* @author 仁平 琉乃
*/

#pragma once
#include "application/GameObject/Player/Bullet/PlayerBullet.h"
#include "engine/TextureManager/TextureManager.h"
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "engine/Sprite/Sprite.h"
#include <vector>
#include "engine/Object3DPlacer/Object3DPlacer.h"
#include "application/GameObject/Rail/Rail.h"
#include "application/AdjustmentVariables/AdjustmentVariables.h"

class LockOn;

class Player : public Collider
{
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
	void OnCollision()override;

	/// <summary>
	/// UIの描画
	/// </summary>
	/// <param name="camera"></param>
	void DrawUI();

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

	Vector3 GetWorldPosition()const override;

	Vector2 GetScreenPosition2DReticle() { return screenPositionReticle_; }

	Vector3 GetRotate()const { return worldTransform_.rotate; }

	bool GetIsHitEnemyFire() { return isHitEnemyFire_; }

	bool GetIsDead() { return isDead_; }

	uint32_t GetHp() { return hp_; }

	float GetDeadTimer() { return deadTimer_; }

	uint32_t GetDestroyCount() { return destroyCount_; }

	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

#pragma endregion

#pragma region setter

	void SetParent(const WorldTransform* parent) { worldTransform_.parent = parent; }

	void SetPosition(const Vector3& position) { worldTransform_.translate = position; }

	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

	void SetPositionZ(float positionZ) { worldTransform_.translate.z = positionZ; }

#pragma endregion

private:
	float moveSpeed_; // 移動スピード
	float bulletSpeed_;
	std::list<std::unique_ptr<PlayerBullet>> bullets_; // 弾のリスト
	uint32_t texHandleBullet_ = 0;
	LockOn* lockOn_ = nullptr;
	Vector2 screenPositionReticle_{};
	WorldTransform worldTransform_{};
	std::unique_ptr<Object3DPlacer> object_ = nullptr;
	bool isDead_ = false;
	bool isHitEnemyFire_ = false;
	int32_t hp_ = 1;
	float deadTimer_ = 60.0f;
	uint32_t destroyCount_ = 0; // 敵を倒した数
};
