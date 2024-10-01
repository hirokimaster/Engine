#pragma once

#include "engine/Base/DX/DirectXCommon.h"
#include "engine/Utility/StringUtility.h"
#include "engine/Utility/ShaderCompile.h"
#include "engine/CreateResource/CreateResource.h"
#include "engine/Math/Vector4.h"

// 共通で使うやつ
struct GraphicsPipelineData {
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;;
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob_ = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_ = nullptr;
};

// pipelineState
struct PipelineState {
	GraphicsPipelineData Object3D;
	GraphicsPipelineData Sprite2D;
	GraphicsPipelineData Particle;
	GraphicsPipelineData PointLight;
	GraphicsPipelineData SpotLight;
	GraphicsPipelineData BlinnPhongObject3D;
	GraphicsPipelineData Bloom;
	GraphicsPipelineData Grayscale;
	GraphicsPipelineData Vignette;
	GraphicsPipelineData GaussianBlur;
	GraphicsPipelineData LuminanceOutline;
	GraphicsPipelineData DepthOutline;
	GraphicsPipelineData RadialBlur;
	GraphicsPipelineData Dissolve;
	GraphicsPipelineData SkinningObject3D;
	GraphicsPipelineData SkyBox;
	GraphicsPipelineData Environment;
	GraphicsPipelineData Line;
	GraphicsPipelineData Random;
};

enum class Light {
	None,
	Point,
	Spot,
	Environment
};

enum class PostEffectType {
	Bloom,
	Grayscale,
	Vignette,
	GaussianBlur,
	LuminanceOutline,
	DepthOutline,
	RadialBlur,
	Dissolve  ,
	Random
};

// BlendMode
enum class BlendMode {
	None,
	Normal,
	Add
};

class GraphicsPipeline {
public:

	// シングルトンインスタンスの取得
	static GraphicsPipeline* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// psoのgetter
	/// </summary>
	/// <returns></returns>
	PipelineState GetPSO() { return  pso; }


private: 
	/// <summary>
	/// rootSignature作成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="descriptionRootSignature"></param>
	/// <param name="property"></param>
	static void CreateRootSignature(Microsoft::WRL::ComPtr <ID3D12Device> device, D3D12_ROOT_SIGNATURE_DESC& descriptionRootSignature, GraphicsPipelineData& data);

	/// <summary>
	/// blendModeの設定
	/// </summary>
	/// <param name="blendDesc"></param>
	static void SetBlendMode(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendMode);

	/// <summary>
	/// パイプラインを実際に生成
	/// </summary>
	/// <param name="pso"></param>
	static void CreatePipeline(PipelineState& pso);

	/// <summary>
	///  パイプラインの種類
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateObject3D(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static GraphicsPipelineData CreateSprite2D(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static GraphicsPipelineData CreateParticle(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static GraphicsPipelineData CreatePointLight(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static GraphicsPipelineData CreateSpotLight(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static GraphicsPipelineData CreateBloom(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static GraphicsPipelineData CreateGrayscale(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static GraphicsPipelineData CreateVignette(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static GraphicsPipelineData CreateGaussianBlur(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static GraphicsPipelineData CreateSkinningObject3D(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static GraphicsPipelineData CreateSkyBox(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static GraphicsPipelineData CreateEnvironment(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static GraphicsPipelineData CreateLine(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static GraphicsPipelineData CreateLuminanceOutline(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static GraphicsPipelineData CreateDepthOutline(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static GraphicsPipelineData CreateRadialBlur(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static GraphicsPipelineData CreateDissolve(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	static GraphicsPipelineData CreateRandom(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	PipelineState pso = {};

};
