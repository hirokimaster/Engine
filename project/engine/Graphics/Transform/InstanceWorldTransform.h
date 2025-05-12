/**
* @file InstanceWorldTransform.h
* @brief instancing用のワールド座標
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Base/DX/DirectXCommon.h"
#include "engine/Math/Mathfunction.h"

struct InstanceWorldTransformForGPU {
	Matrix4x4 matWorld; // ローカル → ワールド変換行列
	Matrix4x4 world; // world
	Matrix4x4 WorldInverseTranspose;//worldの逆行列
	Vector4 color; // 色
};

struct InstanceWorldTransform {
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
	const InstanceWorldTransform* parent = nullptr;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 行列の更新
	/// </summary>
	void UpdateMatrix();
};
