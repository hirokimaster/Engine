/**
* @file Camera.h
* @brief カメラの初期化、生成
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Math/Mathfunction.h"
#include "engine/Graphics/CreateResource/CreateResource.h"

struct Camera {

	// 定数バッファ用データ構造体
	struct ConstBufferDataViewProjection {
		Matrix4x4 view;       // ワールド → ビュー変換行列
		Matrix4x4 projection; // ビュー → プロジェクション変換行列
		Vector3 cameraPos;    // カメラ座標（ワールド座標）
	};

	// 定数バッファ用データ構造体 (sprite用)
	struct ConstBufferDataViewProjectionSprite {
		Matrix4x4 view;       // ワールド → ビュー変換行列
		Matrix4x4 projection; // ビュー → プロジェクション変換行列
		Vector3 cameraPos;    // カメラ座標（ワールド座標）
	};

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	Microsoft::WRL::ComPtr<ID3D12Resource> sConstBuff;
	// マッピング済みアドレス
	ConstBufferDataViewProjection* constMap = nullptr;
	ConstBufferDataViewProjectionSprite* sConstMap = nullptr;


#pragma region	ビュー行列の設定

	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotate = { 0,0,0 };
	Vector3 translate = { 0,0,-60.0f };

#pragma endregion

#pragma region 射影行列の設定

	float fov = 0.45f;
	// ビューポートのアスペクト比
	float aspectRatio = (float)16 / 9;
	// 深度限界（手前側）
	float nearZ = 0.1f;
	// 深度限界（奥側）
	float farZ = 110000.0f;

#pragma endregion

	// ビュー行列
	Matrix4x4 matView = {};
	Matrix4x4 sMatView = {};
	// 射影行列
	Matrix4x4 matProjection = {};
	Matrix4x4 sMatProjection = {};

	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を更新する
	/// </summary>
	void UpdateMatrix();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();
};