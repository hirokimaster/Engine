/**
* @file PlaneProjectionShadow.h
* @brief 影
* @author 仁平 琉乃
*/

#include "engine/3d/BaseObject/BaseInstancingObject.h"

#pragma once
class PlaneProjectionShadow : public BaseInstancingObject
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::string& modelName, const WorldTransform* casterWorldTransform);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const Camera& camera);

private:

	// 投影元オブジェクトのワールド変換データ
	const WorldTransform* casterWorldTransform_ = nullptr;
	// 平面投影行列
	Matrix4x4 shadowMatrix_;

};

