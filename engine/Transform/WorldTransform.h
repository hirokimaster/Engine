#pragma once
#include "engine/Base/DX/DirectXCommon.h"
#include "engine/Math/Mathfunction.h"
#include "engine/CreateResource/CreateResource.h"
#include "engine/Camera/Camera.h"


struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};

struct WorldTransform {

	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotate = { 0.0f,0.0f,0.0f };
	Vector3 translate = { 0.0f,0.0f,0.0f };

	Matrix4x4 matWorld = {};
	Matrix4x4 worldMatrix = {};
	Matrix4x4 sMatWorld = {};
	Matrix4x4 sWorldMatrix = {};

	const WorldTransform* parent = nullptr;

	void Initialize();

	void TransferMatrix(Microsoft::WRL::ComPtr<ID3D12Resource>& wvpResource, Camera& camera);

	void STransferMatrix(Microsoft::WRL::ComPtr<ID3D12Resource>& wvpResource, Camera& camera);

	void UpdateMatrix();
	
};
