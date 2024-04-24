#pragma once
#include "engine/Base/DX/DirectXCommon.h"
#include "engine/Math/Mathfunction.h"
#include "engine/CreateResource/CreateResource.h"
#include "engine/Camera/Camera.h"

struct ConstBufferDataWorldTransform {
	Matrix4x4 matWorld; // ローカル → ワールド変換行列
	Matrix4x4 world; // world
	Matrix4x4 WorldInverseTranspose;//worldの逆行列
};

struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
	// マッピング済みアドレス
	ConstBufferDataWorldTransform* constMap = nullptr;
	// ローカルスケール
	Vector3 scale = { 1.0f, 1.0f, 1.0f };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotate = { 0.0f, 0.0f, 0.0f };
	// ローカル座標
	Vector3 translate = { 0.0f, 0.0f, 0.0f };
	// クォータニオン
	Quaternion quaternion = { 0.0f,0.0f,0.0f,1.0f };
	// world
	Matrix4x4 world;
	// ローカル → ワールド変換行列
	Matrix4x4 matWorld;
	// 親となるワールド変換へのポインタ
	const WorldTransform* parent = nullptr;

	/// <summary>
	/// 初期化
	/// </summary>
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
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();

	/// <summary>
	/// 行列の更新
	/// </summary>
	void UpdateMatrix();

};
