#pragma once
#include "application/GameState/IGameState.h"
#include "application/GameObject/CameraManager/CameraManager.h"

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
