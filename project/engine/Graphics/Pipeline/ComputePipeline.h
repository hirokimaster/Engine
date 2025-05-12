/**
* @file ComputePipeline.h
* @brief コンピュートシェーダーの読み込み
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Base/DX/DirectXCommon.h"
#include "engine/Utility/StringUtility.h"
#include "engine/Utility/ShaderCompile.h"
#include "engine/Graphics/CreateResource/CreateResource.h"
#include "engine/Math/Vector4.h"

struct ComputePipelineData {
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> computePipelineState = nullptr;;
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
};
struct ComputePipelineType {
	ComputePipelineData initializeParticle;
	ComputePipelineData emitterParticle;
	ComputePipelineData updateParticle;
};

class ComputePipeline {
public:

	// シングルトンインスタンスの取得
	static ComputePipeline* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// pipelineの情報取得
	/// </summary>
	/// <returns></returns>
	ComputePipelineType GetPipelineType() { return type_; }

private:
	/// <summary>
	/// pipeline作成
	/// </summary>
	static void CreatePipeline(ComputePipelineType& type);

	/// <summary>
	/// rootSignature作成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="descriptionRootSignature"></param>
	/// <param name="computeState"></param>
	static void CreateRootSignature(Microsoft::WRL::ComPtr <ID3D12Device> device, D3D12_ROOT_SIGNATURE_DESC& descriptionRootSignature, ComputePipelineData& computeState);

	/// <summary>
	/// pipelineState作成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static ComputePipelineData CreateInitializeParticle(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static ComputePipelineData CreateEmitterParticle(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static ComputePipelineData CreateUpdateParticle(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

private:

	ComputePipelineType type_ = {};

};

