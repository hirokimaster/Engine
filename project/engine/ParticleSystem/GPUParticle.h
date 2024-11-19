#pragma once
#include "engine/Math/Vector3.h"
#include "engine/Math/Vector4.h"
#include "engine/Math/Matrix4x4.h"
#include "engine/CreateResource/CreateResource.h"

struct GPUParticle {
	Vector3 translate;
	Vector3 scale;
	float lifeTime;
	Vector3 velocity;
	float currentTime;
	Vector4 color;
};

struct PerView {
	Matrix4x4 viewProjection;
	Matrix4x4 billboardMatrix;
};

class GPUParticle {
public:

	void Initialize();

	void Update();

	void Draw();

private:
	/// <summary>
	/// UAV作成
	/// </summary>
	void CreateUAV();

private:
	// uav用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> uavResource_;

};
