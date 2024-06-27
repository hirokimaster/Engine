#pragma once
#include "application/GameObject/BaseObject/BaseObject.h"

class PlayerBullet : public BaseObject{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="object"></param>
	/// <param name="texHandle"></param>
	/// <param name="model"></param>
	void Initialize(Object3DPlacer* object, uint32_t texHandle, const std::string& model)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(Camera& camera)override;

private: // クラス内でしか使わない

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 消す
	/// </summary>
	void BulletErase();

public:

#pragma	region getter

	bool GetIsDead() { return isDead_; }; // デスフラグ

#pragma endregion

#pragma region	setter

	void SetVelocity(Vector3 velocity) { velocity_ = velocity; }; // 速度ベクトル

	void SetPosition(Vector3 position) { worldTransform_.translate = position; } // 位置

#pragma endregion

private:
	Vector3 velocity_ = {};	// 移動ベクトル
	bool isDead_ = false; // bulletのデスフラグ
	static const int32_t kLifeTime_ = 60 * 5; // 生きてる時間
	int32_t deathTimer_ = kLifeTime_; // デスタイマー
};
