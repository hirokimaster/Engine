/**
* @file CreateResource.h
* @brief バッファリソースの作成
* @author 仁平 琉乃
*/


#pragma once
#include "engine/Base/DX/DirectXCommon.h"
#include "engine/Math/Mathfunction.h"
#include "engine/Math/Vector4.h"
#include "engine/Math/Vector3.h"
#include "engine/Math/Vector2.h"
#include "engine/Math/Matrix3x3.h"

struct Resource {
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource;
};

struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
	float shininess;
	float environmentCoefficient;
};

struct DirectionalLight {
	Vector4 color; // ライトの色
	Vector3 direction; // ライトの向き
	float intensity; // 輝度
};

struct ParticleForGPU {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Vector4 color;
};

struct PointLight {
	Vector4 color;
	Vector3 position;
	float intensity;
	float radius;
	float decay;
	float padding[2];
};

struct SpotLight {
	Vector4 color;
	Vector3 position;
	float intensity;
	Vector3 direction;
	float distance;
	float decay;
	float cosAngle;
	float cosFallooffStart;
	float padding[2];
};

struct CameraData {
	Vector3 worldPosition;
};

struct BloomParam {
	float stepWidth;
	float sigma;
	float lightStrength;
	float bloomThreshold;
};

struct VignetteParam {
	float scale;
	float exponent;
};

struct GaussianParam {
	float sigma;
	float stepWidth;
};

struct ProjectionInverse {
	Matrix4x4 projectionInverse;
};

struct RadialParam {
	Vector2 center;
	float blurWidth;
};

struct DissolveParam {
	float threshold;
};

struct RandomParam {
	float time;
};

class CreateResource {
public:

	/// <summary>
	/// resource作成
	/// </summary>
	/// <param name="sizeInBytes"></param>
	/// <returns></returns>
	static Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	/// <summary>
	/// 中間リソース作成
	/// </summary>
	/// <param name="sizeInBytes"></param>
	/// <returns></returns>
	static Microsoft::WRL::ComPtr<ID3D12Resource> CreateIntermediateResource(size_t sizeInBytes);

	/// <summary>
	/// uav用のリソース作成
	/// </summary>
	/// <param name="sizeInBytes"></param>
	/// <returns></returns>
	static Microsoft::WRL::ComPtr<ID3D12Resource> CreateUAVResource(size_t sizeInBytes);

	/// <summary>
	/// structuredBuffer
	/// </summary>
	/// <param name="sizeInBytes"></param>
	/// <returns></returns>
	static Microsoft::WRL::ComPtr<ID3D12Resource> CreateRWStructuredBufferResource(size_t sizeInBytes);

	/// <summary>
	/// indexBufferView作成
	/// </summary>
	/// <param name="resource"></param>
	/// <param name="sizeInBytes"></param>
	/// <returns></returns>
	static D3D12_INDEX_BUFFER_VIEW CreateIndexBufferView(Microsoft::WRL::ComPtr<ID3D12Resource> resource, size_t sizeInBytes);
	
	/// <summary>
	/// vertexBufferView作成
	/// </summary>
	/// <param name="resource"></param>
	/// <param name="sizeInBytes"></param>
	/// <param name="size"></param>
	/// <returns></returns>
	static D3D12_VERTEX_BUFFER_VIEW CreateVertexBufferView(Microsoft::WRL::ComPtr<ID3D12Resource> resource, size_t sizeInBytes, int size);

};
