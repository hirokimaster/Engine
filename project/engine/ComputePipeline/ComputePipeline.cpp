

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
	type.skinning = CreateSkinning(device, L"Skinning");
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

ComputePipelineData ComputePipeline::CreateSkinning(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName)
{
	ComputePipelineData computeState{};

	HRESULT hr;
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
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, computeState);

	// DescriptorRange
	// uav
	D3D12_DESCRIPTOR_RANGE descriptorRange[2] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV; // UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算
	// srv
	descriptorRange[1].BaseShaderRegister = 1; // 1から始まる
	descriptorRange[1].NumDescriptors = 1; // 数は1つ
	descriptorRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// RootParameter作成
	D3D12_ROOT_PARAMETER rootParameters[5] = {};
	// gOutputVertices (u0)
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[0].DescriptorTable.pDescriptorRanges = &descriptorRange[0];
	rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
	// gMatrixPalette  (t0)
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[1].DescriptorTable.pDescriptorRanges = &descriptorRange[1];
	rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
	// gInputVertices  (t1)
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[2].DescriptorTable.pDescriptorRanges = &descriptorRange[1];
	rootParameters[2].DescriptorTable.NumDescriptorRanges = 1;
	// gInfluences (t2)
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[3].DescriptorTable.pDescriptorRanges = &descriptorRange[1];
	rootParameters[3].DescriptorTable.NumDescriptorRanges = 1;
	// gSkinningInformation	(b0)
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // PixelShaderで使う
	rootParameters[4].Descriptor.ShaderRegister = 0;

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	IDxcBlob* shaderBlob = nullptr;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	shaderBlob = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".CS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
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
