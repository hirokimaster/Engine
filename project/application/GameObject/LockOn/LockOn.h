#pragma once
#include "application/GameObject/Enemy/Enemy.h"
#include "engine/Sprite/Sprite.h"
#include <math.h>

class Player;

class LockOn {
public:
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="enemies"></param>
	/// <param name="camera"></param>
	void Update(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera);

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
	bool UnLock(const Camera& camera);

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

	Vector3 TransformPositionScreen(const Vector3& positionWorld, const Camera& camera);

	/// <summary>
	/// レティクルの範囲にいるか判定する
	/// </summary>
	/// <param name="position"></param>
	bool CheckReticleRange(const Vector3& position);

public:

#pragma region gettr

	const Vector2& GetPosition() { return positionScreen_; } // reticleのposition

	const Enemy* GetTarget() { return target_; } // targetのポインタ

#pragma endregion

#pragma region setter

	void SetPlayer(Player* player) { player_ = player; }

#pragma endregion

private:
	
	const Enemy* target_ = nullptr;	// ロックオン対象
	std::unique_ptr<Sprite> spriteLockOn_ = nullptr; // ロックオンのsprite
	uint32_t texHandleLockOn_ = 0; // spriteのtexHandle
	float minDistance_ = 10.0f;
	float maxDistance_ = 100.0f;
	Vector2 positionScreen_{};
	Player* player_ = nullptr;
};