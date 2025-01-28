#pragma once
#include "application/GameObject/BaseObject/BaseObject.h"

class Laser : public BaseObject {
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

	Vector3 GetScale() const override { return object_->GetWorldTransform().scale; }

	bool GetIsHit()const { return isHit_; }

#pragma endregion

#pragma region setter

	void SetPosition(Vector3 position)override { object_->SetPosition(position); }

#pragma endregion

private:

	void OnCollision()override;

private:
	bool isHit_ = false;
};
