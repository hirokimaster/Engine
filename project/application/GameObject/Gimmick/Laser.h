#pragma once
#include "application/GameObject/Gimmick/IGimmick.h"

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
	void Draw(const Camera& camera)override;

private:
	uint32_t texHandle_ = 0;
};
