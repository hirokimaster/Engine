#pragma once
#include "engine/Object3DPlacer/Object3DPlacer.h"

class BaseCharacter
{
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="object"></param>
	virtual void Initialize(Object3DPlacer* object, uint32_t texHandle, const std::string& model);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	virtual void Draw(Camera& camera);

#pragma region getter
	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

#pragma endregion


protected:
	Object3DPlacer* object_ = nullptr;
	WorldTransform worldTransform_{};
};

