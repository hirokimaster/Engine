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

#pragma region	setter

	void SetVelocity(Vector3 velocity) { velocity_ = velocity; };

	void SetPosition(Vector3 position) { worldTransform_.translate = position; }

#pragma endregion

private:
	Vector3 velocity_ = {};	// 移動ベクトル

};
