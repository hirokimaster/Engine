#pragma once
#include "application/GameObject/Gimmick/IGimmick/IGimmick.h"

class Laser : public IGimmick {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(Camera& camera)override;

#pragma region getter

	Vector3 GetWorldPosition() const override;

	bool GetIsHit()const { return isHit_; }

#pragma endregion

private:

	void OnCollision()override;

private:
	uint32_t texHandle_ = 0;
	bool isHit_ = false;
};
