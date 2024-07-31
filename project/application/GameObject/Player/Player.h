#pragma once
#include "application/GameObject/Player/Bullet/PlayerBullet.h"
#include "engine/TextureManager/TextureManager.h"
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "engine/Sprite/Sprite.h"
#include <vector>
#include "engine/Object3DPlacer/Object3DPlacer.h"

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

public:

#pragma region getter

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() const { return bullets_; }

	Vector3 GetWorldPosition()const override;

	Vector2 GetScreenPosition2DReticle() { return screenPositionReticle_; }

	Vector3 GetRotate()const { return worldTransform_.rotate; }

#pragma endregion

#pragma region setter

	void SetParent(const WorldTransform* parent) { worldTransform_.parent = parent; }

	void SetPosition(Vector3 position) { worldTransform_.translate = position; }

	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

#pragma endregion

private:
	const float kMoveSpeed_ = 0.2f; // 移動スピード
	std::list<std::unique_ptr<PlayerBullet>> bullets_; // 弾のリスト
	std::list<std::unique_ptr<PlayerBullet>>::iterator bulletsItr_; // 弾のイテレータ
	uint32_t texHandleBullet_ = 0;
	LockOn* lockOn_ = nullptr;
	Vector2 screenPositionReticle_{};
	float shotTimer_ = 0.0f;
	WorldTransform worldTransform_{};
	std::unique_ptr<Object3DPlacer> object_ = nullptr;
};
