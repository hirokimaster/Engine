

#include "ComputePipeline.h"

ComputePipeline* ComputePipeline::GetInstance()
{
	static ComputePipeline instance;
	return &instance;
}

void ComputePipeline::Initialize()
{
	ComputePipelineType type = {};
	CreatePipeline(type);
	ComputePipeline::GetInstance()->type_ = type;
}

void ComputePipeline::CreatePipeline(ComputePipelineType& type)
{
	Microsoft::WRL::ComPtr <ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();
	type.particle = CreateParticle(device, L"InitializeParticle");
}

void ComputePipeline::CreateRootSignature(Microsoft::WRL::ComPtr<ID3D12Device> device, D3D12_ROOT_SIGNATURE_DESC& descriptionRootSignature, ComputePipelineData& computeState)
{
	// シリアライズしてバイナリにする
	HRESULT hr_ = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &computeState.signatureBlob, &computeState.errorBlob);
	if (FAILED(hr_)) {
		Log(reinterpret_cast<char*>(computeState.errorBlob->GetBufferPointer()));
		assert(false);
	}

	// バイナリを元に生成
	hr_ = device->CreateRootSignature(0, computeState.signatureBlob->GetBufferPointer(), computeState.signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&computeState.rootSignature));
	assert(SUCCEEDED(hr_));
}

ComputePipelineData ComputePipeline::CreateParticle(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName)
{
	ComputePipelineData computeState{};

	[[maybe_unused]] HRESULT hr;
	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;;

	// DescriptorRange
	// uav
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV; // UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// RootParameter作成
	D3D12_ROOT_PARAMETER rootParameters[1] = {};
	// gParticle (u0)
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptor Tableとして定義
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // 全てのシェーダーステージでアクセス可能
	rootParameters[0].DescriptorTable.NumDescriptorRanges = 1; // Descriptor Rangeは1つ
	rootParameters[0].DescriptorTable.pDescriptorRanges = &descriptorRange[0]; // Descriptor Rangeを関連付け

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけMipmapを使う
	staticSamplers[0].ShaderRegister = 0;         // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // PixelShaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, computeState);

	IDxcBlob* shaderBlob = nullptr;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	shaderBlob = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".CS.hlsl", L"cs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(shaderBlob != nullptr);

	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipelineStateDesc{};
	computePipelineStateDesc.CS = {
		.pShaderBytecode = shaderBlob->GetBufferPointer(),
		.BytecodeLength = shaderBlob->GetBufferSize()
	};

	// pipelineState作成
	computePipelineStateDesc.pRootSignature = computeState.rootSignature.Get();
	computeState.computePipelineState = nullptr;
	hr = device->CreateComputePipelineState(&computePipelineStateDesc, IID_PPV_ARGS(&computeState.computePipelineState));

	return computeState;
}