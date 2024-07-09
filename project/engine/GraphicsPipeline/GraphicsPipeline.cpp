#include "GraphicsPipeline.h"


GraphicsPipeline* GraphicsPipeline::GetInstance() {
	static GraphicsPipeline instance;
	return &instance;
}

/// <summary>
/// 初期化
/// </summary>
void GraphicsPipeline::Initialize() {	

	PipelineState pso = {};
	CreatePipeline(pso);
	GraphicsPipeline::GetInstance()->pso = pso;
	
}

/// <summary>
/// rootSignature作成
/// </summary>
/// <param name="device"></param>
/// <param name="descriptionRootSignature"></param>
/// <param name="property"></param>
void GraphicsPipeline::CreateRootSignature(Microsoft::WRL::ComPtr <ID3D12Device> device, D3D12_ROOT_SIGNATURE_DESC& descriptionRootSignature, Property& property) {

	// シリアライズしてバイナリにする
	HRESULT hr_ = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &property.signatureBlob_, &property.errorBlob_);
	if (FAILED(hr_)) {
		Log(reinterpret_cast<char*>(property.errorBlob_->GetBufferPointer()));
		assert(false);
	}

	// バイナリを元に生成
	hr_ = device->CreateRootSignature(0, property.signatureBlob_->GetBufferPointer(), property.signatureBlob_->GetBufferSize(),
		IID_PPV_ARGS(&property.rootSignature_));
	assert(SUCCEEDED(hr_));
}

void GraphicsPipeline::SetBlendMode(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendMode){

	// BlendState
	switch (blendMode) {
	case BlendNone:
		blendDesc.BlendEnable = FALSE;

	case BlendNormal:
		blendDesc.BlendEnable = TRUE;
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		break;

	case BlendAdd:
		blendDesc.BlendEnable = TRUE;
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		break;
	}
	
}

/// <summary>
/// パイプライン生成
/// </summary>
/// <param name="ps"></param>
void GraphicsPipeline::CreatePipeline(PipelineState& pso) {

	Microsoft::WRL::ComPtr <ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();

	pso.Object3D = CreateObject3D(device.Get(), L"Object3d");
	pso.Sprite2D = CreateSprite2D(device.Get(), L"Sprite2D");
	pso.Particle = CreateParticle(device.Get(), L"Particle");
	pso.PointLight = CreatePointLight(device.Get(), L"PointLight");
	pso.SpotLight = CreateSpotLight(device.Get(), L"SpotLight");
	pso.Bloom = CreateBloom(device.Get(), L"Bloom");
	pso.Grayscale = CreateGrayscale(device.Get(), L"Grayscale");
	pso.Vignette = CreateVignette(device.Get(), L"Vignette");
	pso.GaussianBlur = CreateGaussianBlur(device.Get(), L"GaussianBlur");
	pso.LuminanceOutline = CreateLuminanceOutline(device.Get(), L"LuminanceBasedOutline");
	pso.SkinningObject3D = CreateSkinningObject3D(device.Get(), L"SkinningObject3d");
	pso.SkyBox = CreateSkyBox(device.Get(), L"SkyBox");
	pso.Environment = CreateEnvironment(device.Get(), L"Environment");
	pso.Line = CreateLine(device.Get(), L"Line");
	pso.DepthOutline = CreateDepthOutline(device.Get(), L"DepthBasedOutline");
	pso.RadialBlur = CreateRadialBlur(device.Get(), L"RadialBlur");
	pso.Dissolve = CreateDissolve(device.Get(), L"Dissolve");
	pso.Random = CreateRandom(device.Get(), L"Random");
}

/*-------------------------------------------------
	      ↓ GraphicsPipelineを分けて作成 ↓
----------------------------------------------------*/

/// <summary>
/// 3dObject,lighting
/// </summary>
/// <param name="device"></param>
/// <param name="shaderName"></param>
/// <returns></returns>
Property GraphicsPipeline::CreateObject3D(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName) {

	Property property;

	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr_));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr_));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// RootParameter作成
	D3D12_ROOT_PARAMETER rootParameters[5] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[2].Descriptor.ShaderRegister = 1;
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderを使う
	rootParameters[4].Descriptor.ShaderRegister = 1; // レジスタ番号1を使う
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);


	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0～1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// rootSignature作成
	CreateRootSignature(device,descriptionRootSignature, property);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);


	// BlendState	
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendMode(blendDesc, BlendNormal);

	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	vertexShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob_ != nullptr);
																
	pixelShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob_ != nullptr);

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = property.rootSignature_.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	return property;
}

/// <summary>
/// 2Dスプライト用
/// </summary>
/// <param name="device"></param>
/// <param name="shaderName"></param>
/// <returns></returns>
Property GraphicsPipeline::CreateSprite2D(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName)
{
	Property property;

	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr_));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr_));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// rootParameters
	D3D12_ROOT_PARAMETER rootParameters[4] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[2].Descriptor.ShaderRegister = 1;
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0～1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, property);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);


	// BlendState	
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendMode(blendDesc, BlendNormal);

	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	vertexShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob_ != nullptr);

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = property.rootSignature_.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	return property;
}

/// <summary>
/// particle
/// </summary>
/// <param name="device"></param>
/// <param name="shaderName"></param>
/// <returns></returns>
Property GraphicsPipeline::CreateParticle(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName) {
	Property property;

	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr_));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr_));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRangeForInstancing[0].NumDescriptors = 1; // 数は1つ
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// RootParameter作成
	D3D12_ROOT_PARAMETER rootParameters[3] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing; // Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing); // Tableで利用する数

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);


	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0～1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, property);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);


	// BlendState	
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendMode(blendDesc, BlendAdd);

	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	vertexShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob_ != nullptr);

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = property.rootSignature_.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	return property;
}

/// <summary>
/// pointLight
/// </summary>
/// <param name="device"></param>
/// <param name="shaderName"></param>
/// <returns></returns>
Property GraphicsPipeline::CreatePointLight(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName)
{
	Property property;

	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr_));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr_));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// RootParameter作成
	D3D12_ROOT_PARAMETER rootParameters[7] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[2].Descriptor.ShaderRegister = 1;
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderを使う
	rootParameters[4].Descriptor.ShaderRegister = 1; // レジスタ番号1を使う
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderを使う
	rootParameters[5].Descriptor.ShaderRegister = 2; // レジスタ番号2を使う
	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderを使う
	rootParameters[6].Descriptor.ShaderRegister = 3; // レジスタ番号3を使う
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);


	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0～1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, property);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[4] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[3].SemanticName = "WPOSITION";
	inputElementDescs[3].SemanticIndex = 0;
	inputElementDescs[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);


	// BlendState	
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendMode(blendDesc, BlendNormal);

	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	vertexShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob_ != nullptr);

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = property.rootSignature_.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	return property;
}

/// <summary>
/// spotlight
/// </summary>
/// <param name="device"></param>
/// <param name="shaderName"></param>
/// <returns></returns>
Property GraphicsPipeline::CreateSpotLight(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName)
{
	Property property;

	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr_));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr_));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// RootParameter作成
	D3D12_ROOT_PARAMETER rootParameters[7] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[2].Descriptor.ShaderRegister = 1;
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderを使う
	rootParameters[4].Descriptor.ShaderRegister = 1; // レジスタ番号1を使う
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderを使う
	rootParameters[5].Descriptor.ShaderRegister = 2; // レジスタ番号2を使う
	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderを使う
	rootParameters[6].Descriptor.ShaderRegister = 3; // レジスタ番号3を使う
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);


	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0～1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, property);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[4] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[3].SemanticName = "WPOSITION";
	inputElementDescs[3].SemanticIndex = 0;
	inputElementDescs[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);


	// BlendState	
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendMode(blendDesc, BlendNormal);

	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	vertexShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob_ != nullptr);

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = property.rootSignature_.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	return property;
}

/// <summary>
/// postEffectTest用
/// </summary>
/// <param name="device"></param>
/// <param name="shaderName"></param>
/// <returns></returns>
Property GraphicsPipeline::CreateBloom(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName)
{
	Property property;

	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr_));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr_));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// rootParameters
	D3D12_ROOT_PARAMETER rootParameters[2] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP; // 0～1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, property);

	// InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;


	// BlendState	
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendMode(blendDesc, BlendNone);

	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	vertexShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/PostProcess.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob_ != nullptr);

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = property.rootSignature_.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = false;

	// 実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	return property;
}

/// <summary>
/// grayscale
/// </summary>
/// <param name="device"></param>
/// <param name="shaderName"></param>
/// <returns></returns>
Property GraphicsPipeline::CreateGrayscale(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName)
{
	Property property;

	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr_));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr_));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// rootParameters
	D3D12_ROOT_PARAMETER rootParameters[1] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP; // 0～1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, property);

	// InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;


	// BlendState	
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendMode(blendDesc, BlendNone);

	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	vertexShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/PostProcess.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob_ != nullptr);

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = property.rootSignature_.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = false;

	// 実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	return property;
}

/// <summary>
/// vignette
/// </summary>
/// <param name="device"></param>
/// <param name="shaderName"></param>
/// <returns></returns>
Property GraphicsPipeline::CreateVignette(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName)
{
	Property property;

	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr_));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr_));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// rootParameters
	D3D12_ROOT_PARAMETER rootParameters[2] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP; // 0～1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, property);

	// InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;


	// BlendState	
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendMode(blendDesc, BlendNone);

	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	vertexShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/PostProcess.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob_ != nullptr);

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = property.rootSignature_.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = false;

	// 実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	return property;
}

/// <summary>
/// gaussianblur
/// </summary>
/// <param name="device"></param>
/// <param name="shaderName"></param>
/// <returns></returns>
Property GraphicsPipeline::CreateGaussianBlur(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName)
{
	Property property;

	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr_));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr_));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// rootParameters
	D3D12_ROOT_PARAMETER rootParameters[2] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP; // 0～1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, property);

	// InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;


	// BlendState	
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendMode(blendDesc, BlendNone);

	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	vertexShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/PostProcess.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob_ != nullptr);

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = property.rootSignature_.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = false;
	
	// 実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	return property;
}

/// <summary>
/// skinning用
/// </summary>
/// <param name="device"></param>
/// <param name="shaderName"></param>
/// <returns></returns>
Property GraphicsPipeline::CreateSkinningObject3D(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName)
{
	Property property;

	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr_));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr_));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// RootParameter作成
	D3D12_ROOT_PARAMETER rootParameters[6] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[2].Descriptor.ShaderRegister = 1;
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[3].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[4].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	rootParameters[4].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderを使う
	rootParameters[5].Descriptor.ShaderRegister = 1; // レジスタ番号1を使う
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);


	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0～1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, property);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[5] = {};

	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[3].SemanticName = "WEIGHT";
	inputElementDescs[3].SemanticIndex = 0;
	inputElementDescs[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[3].InputSlot = 1;
	inputElementDescs[3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[4].SemanticName = "INDEX";
	inputElementDescs[4].SemanticIndex = 0;
	inputElementDescs[4].Format = DXGI_FORMAT_R32G32B32A32_SINT;
	inputElementDescs[4].InputSlot = 1;
	inputElementDescs[4].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);


	// BlendState	
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendMode(blendDesc, BlendNormal);

	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	vertexShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/Object3d.PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob_ != nullptr);

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = property.rootSignature_.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = false;

	// 実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	return property;
}

/// <summary>
/// 輝度ベースのアウトライン
/// </summary>
/// <param name="device"></param>
/// <param name="shaderName"></param>
/// <returns></returns>
Property GraphicsPipeline::CreateLuminanceOutline(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName)
{
	Property property;

	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr_));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr_));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// rootParameters
	D3D12_ROOT_PARAMETER rootParameters[1] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP; // 0～1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, property);

	// InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;


	// BlendState	
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendMode(blendDesc, BlendNormal);

	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	vertexShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/PostProcess.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob_ != nullptr);

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = property.rootSignature_.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = false;
	
	// 実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	return property;
}

/// <summary>
/// 深度値ベースのアウトライン
/// </summary>
/// <param name="device"></param>
/// <param name="shaderName"></param>
/// <returns></returns>
Property GraphicsPipeline::CreateDepthOutline(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName)
{
	Property property;

	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr_));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr_));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[2] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算
	// depthのtexture用
	descriptorRange[1].BaseShaderRegister = 1; // 0から始まる
	descriptorRange[1].NumDescriptors = 1; // 数は1つ
	descriptorRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// rootParameters
	D3D12_ROOT_PARAMETER rootParameters[3] = {};
	// texture
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = &descriptorRange[0]; // Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = 1; // Tableで利用する数
	// projection
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	// depth用のtexture
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = &descriptorRange[1]; // Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = 1; // Tableで利用する数

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[2] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP; // 0～1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	// depth用
	staticSamplers[1].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT; // バイリニアフィルタ
	staticSamplers[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP; // 0～1の範囲をリピート
	staticSamplers[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[1].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[1].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[1].ShaderRegister = 1; // レジスタ番号1を使う
	staticSamplers[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, property);

	// InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;


	// BlendState	
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendMode(blendDesc, BlendNone);

	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	vertexShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/PostProcess.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob_ != nullptr);

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = property.rootSignature_.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = false;

	// 実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	return property;
}

/// <summary>
/// radialblur
/// </summary>
/// <param name="device"></param>
/// <param name="shaderName"></param>
/// <returns></returns>
Property GraphicsPipeline::CreateRadialBlur(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName)
{
	Property property;

	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr_));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr_));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// rootParameters
	D3D12_ROOT_PARAMETER rootParameters[2] = {};
	// texture
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数
	// cbuffer(中心点,ぼかし幅)
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP; // 0～1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, property);

	// InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;


	// BlendState	
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendMode(blendDesc, BlendNone);

	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	vertexShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/PostProcess.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob_ != nullptr);

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = property.rootSignature_.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = false;

	// 実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	return property;
}

Property GraphicsPipeline::CreateDissolve(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName)
{
	Property property;

	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr_));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr_));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[2] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算
	// maskTexture
	descriptorRange[1].BaseShaderRegister = 1; // 1から始まる
	descriptorRange[1].NumDescriptors = 1; // 数は1つ
	descriptorRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// rootParameters
	D3D12_ROOT_PARAMETER rootParameters[3] = {};
	// texture
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = &descriptorRange[0]; // Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = 1; // Tableで利用する数
	// cbuffer(mask)
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	// maskTexture
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = &descriptorRange[1]; // Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = 1; // Tableで利用する数
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP; // 0～1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, property);

	// InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;


	// BlendState	
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendMode(blendDesc, BlendNone);

	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	vertexShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/PostProcess.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob_ != nullptr);

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = property.rootSignature_.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = false;

	// 実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	return property;
}

/// <summary>
/// random
/// </summary>
/// <param name="device"></param>
/// <param name="shaderName"></param>
/// <returns></returns>
Property GraphicsPipeline::CreateRandom(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName)
{
	
	Property property;

	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr_));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr_));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// rootParameters
	D3D12_ROOT_PARAMETER rootParameters[2] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数
	// time
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP; // 0～1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, property);

	// InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;


	// BlendState	
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendMode(blendDesc, BlendNone);

	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	vertexShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/PostProcess.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob_ != nullptr);

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = property.rootSignature_.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = false;

	// 実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	return property;
}


/// <summary>
/// skyBox用
/// </summary>
/// <param name="device"></param>
/// <param name="shaderName"></param>
/// <returns></returns>
Property GraphicsPipeline::CreateSkyBox(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName)
{
	Property property;

	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr_));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr_));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// RootParameter作成
	D3D12_ROOT_PARAMETER rootParameters[4] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[2].Descriptor.ShaderRegister = 1;
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);


	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0～1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, property);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);


	// BlendState	
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendMode(blendDesc, BlendNormal);

	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	vertexShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob_ != nullptr);

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = property.rootSignature_.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	return property;	
}

Property GraphicsPipeline::CreateEnvironment(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName)
{
	Property property;

	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr_));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr_));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[2] = {};
	// texture
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算
	// 環境マップのtexture
	descriptorRange[1].BaseShaderRegister = 1; // 1から始まる
	descriptorRange[1].NumDescriptors = 1; // 数は1つ
	descriptorRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算


	// RootParameter作成
	D3D12_ROOT_PARAMETER rootParameters[7] = {};
	// material
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	// transformationMatrix
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	// viewProjection
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[2].Descriptor.ShaderRegister = 1;
	// texture
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = &descriptorRange[0]; // Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = 1; // Tableで利用する数
	// directionalLight
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[4].Descriptor.ShaderRegister = 1;
	// cameraData
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[5].Descriptor.ShaderRegister = 2;
	// 環境マップのtexture
	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[6].DescriptorTable.pDescriptorRanges = &descriptorRange[1]; // Tableの中身の配列を指定
	rootParameters[6].DescriptorTable.NumDescriptorRanges = 1; // Tableで利用する数
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);


	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0～1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, property);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[4] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[3].SemanticName = "WPOSITION";
	inputElementDescs[3].SemanticIndex = 0;
	inputElementDescs[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);


	// BlendState	
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendMode(blendDesc, BlendNone);

	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	vertexShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob_ != nullptr);

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = property.rootSignature_.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	return property;
}

/// <summary>
/// line用
/// </summary>
/// <param name="device"></param>
/// <param name="shaderName"></param>
/// <returns></returns>
Property GraphicsPipeline::CreateLine(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& shaderName)
{
	Property property;

	// dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr_));

	// 現時点ではincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr_));

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// RootParameter作成
	D3D12_ROOT_PARAMETER rootParameters[3] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[2].Descriptor.ShaderRegister = 1;
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);


	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0～1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // pixelshaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// rootSignature作成
	CreateRootSignature(device, descriptionRootSignature, property);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);


	// BlendState	
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendMode(blendDesc, BlendNormal);

	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	ShaderCompile* compile_ = nullptr;
	// shaderをコンパイルする
	vertexShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/Object3d.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = compile_->CompileShader(L"resources/ShaderFile/" + shaderName + L".PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob_ != nullptr);

	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = property.rootSignature_.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	return property;
}

/*-------------------------------------------------
			  ↑  ここまで  ↑
----------------------------------------------------*/
