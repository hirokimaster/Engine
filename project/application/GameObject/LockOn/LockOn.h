#pragma once
#include "application/GameObject/Enemy/Enemy.h"
#include "engine/Sprite/Sprite.h"
#include <math.h>

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
	/// 距離条件の判定
	/// </summary>
	/// <param name="positionView"></param>
	/// <returns></returns>
	bool CheckDistance(const Vector3& positionView);

	/// <summary>
	/// worldからviewに変換
	/// </summary>
	/// <param name="positionWorld"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	Vector3 TransformPositionWorld(const Vector3& positionWorld, const Camera& camera);

private:
	
	const Enemy* target_ = nullptr;	// ロックオン対象
	std::unique_ptr<Sprite> spriteLockOn_ = nullptr; // ロックオンのsprite
	uint32_t texHandleLockOn_ = 0; // spriteのtexHandle
	float minDistance_ = 10.0f;
	float maxDistance_ = 100.0f;
};