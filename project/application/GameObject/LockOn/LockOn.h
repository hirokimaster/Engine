/**
* @file LockOn.h
* @brief プレイヤーのロックオン機能、レティクル
* @author 仁平 琉乃
*/

#pragma once
#include "application/GameObject/Enemy/Enemy.h"
#include "engine/Sprite/Sprite.h"
#include <math.h>
#include "engine/Math/Mathfunction.h"

class LockOn {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="enemies"></param>
	/// <param name="camera"></param>
	void Update(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// レティクルの更新
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="playerPosition"></param>
	void UpdateReticle(const Camera& camera, const Vector3& playerPosition, bool isGameStart);

private:

	/// <summary>
	/// 敵を検索
	/// </summary>
	/// <param name="enemies"></param>
	/// <param name="camera"></param>
	void Search(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera);

	/// <summary>
	/// ロックオン解除
	/// </summary>
	bool UnLock(const Camera& camera, const Enemy* target);

	/// <summary>
	/// reticleのposition計算
	/// </summary>
	void ReticlePositionCalc(const Camera& camera);

	/// <summary>
	/// worldからviewに変換
	/// </summary>
	/// <param name="positionWorld"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	Vector3 TransformPositionWorld(const Vector3& positionWorld, const Camera& camera);

	/// <summary>
	/// worldからscreenに変換
	/// </summary>
	/// <param name="positionWorld"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	Vector3 TransformPositionScreen(const Vector3& positionWorld, const Camera& camera);

	/// <summary>
	/// レティクルの範囲にいるか判定する
	/// </summary>
	/// <param name="position"></param>
	bool CheckReticleRange(const Vector3& position);

	/// <summary>
	/// レティクル
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="position"></param>
	/// <param name="playerPosition"></param>
	void Reticle(const Camera& camera, const Vector2& position, const Vector3& playerPosition);

	/// <summary>
	/// レティクルの範囲制限
	/// </summary>
	void ReticleRangeLimit();

public:

#pragma region getter

	const std::list<Vector2> GetPosition() { return positionScreen_; } // reticleのposition

	std::list<const Enemy*> GetTarget() { return target_; } // targetのポインタ

	Vector3 GetWorldPosition3DReticle()const; // reticleのworld座標

	bool GetIsLockOnMode()const { return isLockOnMode_; } // ロックオンのフラグ

	Vector3 GetWorldTransform() { return worldTransform3DReticle_.translate; }

#pragma endregion

#pragma region setter

	void SetIsLockOnMode(bool lockOnMode) { isLockOnMode_ = lockOnMode; }

#pragma endregion

private:

	std::list<const Enemy*> target_;	// ロックオン対象
	float minDistance_ = 10.0f;
	float maxDistance_ = 100.0f;
	std::list<Vector2> positionScreen_{};
	std::list<std::unique_ptr<Sprite>> sprite_;
	uint32_t texHandle2DReticle_ = 0;
	WorldTransform worldTransform3DReticle_; // reticleのworldTransform
	std::unique_ptr<Sprite> sprite2DReticle_;
	Vector2 screenPositionReticle_{};
	bool isLockOnMode_ = false;
	float lockOnTimer_ = 60.0f;
	bool startLockOnTimer_ = false;
	uint32_t texHandleLockOnReticle_ = 0;
	std::unique_ptr<Sprite> spriteLockOnReticle_;
	Vector2 reticlePosition_ = { 640.0f, 360.0f }; // reticleの範囲制限用のposition
	std::unique_ptr<Object3DPlacer> debugReticle_ = nullptr;
};
