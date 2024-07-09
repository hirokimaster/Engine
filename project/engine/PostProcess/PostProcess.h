#pragma once
#include "engine/Sprite/Sprite.h"
#include "engine/DescriptorManager/DescriptorManager.h"

class PostProcess{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostProcess();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostProcess();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

#pragma region setter

	void SetEffect(PostEffectType type);

	BloomParam SetBloomParam(BloomParam bloom) { return *bloomData_ = bloom; }

	GaussianParam SetGaussianParam(GaussianParam gaussian) { return *gaussianData_ = gaussian; }

	RadialParam SetRadialParam(RadialParam param) { return *radialData_ = param; }

	DissolveParam SetDissolveParam(DissolveParam param) { return *dissolveData_ = param; }

	RandomParam SetRandomParam(RandomParam param) {return *randomData_ = param;}

	void SetMaskTexture(uint32_t texHandle) {maskTexHandle_ = texHandle; }

	void SetCamera(Camera camera) { camera = camera_; }

	PostEffectType GetEffectType() { return type_; }

#pragma endregion

private: // このクラス内でしか使わない関数
	/// <summary>
	/// rtv作成
	/// </summary>
	void CreateRTV();

	/// <summary>
	/// srv作成
	/// </summary>
	void CreateSRV();

	/// <summary>
	/// buffer作成
	/// </summary>
	void CreateBuffer();

	/// <summary>
	/// pipelineを作成
	/// </summary>
	void CreatePipeLine();

	/// <summary>
	/// constantBufferViewの設定
	/// </summary>
	void SetConstantBuffer();

	/// <summary>
	/// depth用のtextureのsrv
	/// </summary>
	void CreateDepthTextureSrv();

	/// <summary>
	/// depthTextureSRV用のバリア
	/// </summary>
	void PreDepthBarrier();
	void PostDepthBarrier();

private:
	Resource resource_{};
	Vector4* materialData_ = nullptr;
	uint32_t texHandle_ = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff_;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer_;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_;
	D3D12_RESOURCE_BARRIER barrier_{};
	D3D12_RESOURCE_BARRIER depthBarrier_{};
	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorRect{};
	uint32_t index_;
	BloomParam* bloomData_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> bloom_;
	VignetteParam* vignetteData_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> vignette_;
	GaussianParam* gaussianData_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> gaussian_;
	PostEffectType type_;
	Property pipeline_;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthTexBuff_;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthOutline_;
	ProjectionInverse* projection_ = nullptr;
	Camera camera_{};
	Microsoft::WRL::ComPtr<ID3D12Resource> radialBlur_;
	RadialParam* radialData_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> dissolve_;
	DissolveParam* dissolveData_ = nullptr;
	uint32_t maskTexHandle_ = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> random_;
	RandomParam* randomData_ = nullptr;

};
