/**
* @file GraphicsPipeline.h
* @brief グラフィックスパイプラインの初期化、生成。シェーダーの読み込み
* @author 仁平 琉乃
*/

#pragma once

#include "engine/Base/DX/DirectXCommon.h"
#include "engine/Utility/StringUtility.h"
#include "engine/Utility/ShaderCompile.h"
#include "engine/Graphics/CreateResource/CreateResource.h"
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
	GraphicsPipelineData Object3DInstancing;
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
	GraphicsPipelineData gpuParticle;
	GraphicsPipelineData postEffectNone;
};

enum class Light {
	None,
	Point,
	Spot,
	Environment
};

enum class PostEffectType {
	None,
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

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
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

// シェーダ読み込み、グラフィックスパイプライン生成
#pragma region 

	/// <summary>
	///  object3D
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateObject3D(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// インスタンシング用のobject3D
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateObject3DInstancing(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// sprite2D
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateSprite2D(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// particle
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateParticle(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// pointlight
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreatePointLight(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// spotlight
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateSpotLight(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// bloom
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateBloom(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// grayscale
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateGrayscale(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// vignette
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateVignette(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// gaussianBlur
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateGaussianBlur(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// skinningObject3D
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateSkinningObject3D(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// skyBox
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateSkyBox(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// enviroment
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateEnvironment(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// line
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateLine(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// luminanceOutline
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateLuminanceOutline(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// depthOutline
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateDepthOutline(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// radialBlur
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateRadialBlur(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// dissolve
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateDissolve(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// random
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateRandom(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// random
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreateGPUParticle(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

	/// <summary>
	/// postEffectかけない
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static GraphicsPipelineData CreatePostEffectNone(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);

#pragma endregion

private:

	GraphicsPipeline() = default;
	~GraphicsPipeline() = default;
	GraphicsPipeline(const GraphicsPipeline&) = delete;
	const GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

private: // メンバ変数

	PipelineState pso = {};

};
