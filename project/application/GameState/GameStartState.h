/**
* @file GameStartState.h
* @brief ゲームスタート演出
* @author 仁平 琉乃
*/

#pragma once
#include "application/GameState/IGameState.h"
#include "application/GameObject/CameraManager/CameraManager.h"
#include "application/AdjustmentVariables/AdjustmentVariables.h"

class GameStartState : public IGameState {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

private:

	/// <summary>
	/// 調整項目の追加
	/// </summary>
	void AddAdjustmentVariables();

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyAdjustmentVariables();

private:
	// カメラワーク用
   // 初期のカメラのオフセットと回転
	Vector3 offsetStart_ = {};
	Vector3 cameraRotateStart_{};
	// 目的のカメラのオフセットと回転
	Vector3 offsetEnd_{};
	Vector3 cameraRotateEnd_{};
	float rotateParam_ = 0.0f;
	CameraManager* cameraManager_ = nullptr; // カメラマネージャのポインタ
};
