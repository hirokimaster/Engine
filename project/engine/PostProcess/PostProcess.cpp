#include "PostProcess.h"

PostProcess::PostProcess()
{
}

PostProcess::~PostProcess()
{
}

void PostProcess::Initialize()
{
	// srvを作るところを今使ってるところの隣にずらす
	SrvManager::GetInstance()->ShiftIndex();
	index_ = SrvManager::GetInstance()->GetIndex();
	CreateSRV();
	CreateRTV();

	// クライアント領域のサイズと一緒にして画面全体に表示
	viewport.Width = WinApp::kWindowWidth;
	viewport.Height = WinApp::kWindowHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// 基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = WinApp::kWindowWidth;
	scissorRect.top = 0;
	scissorRect.bottom = WinApp::kWindowHeight;
}

void PostProcess::CreateRTV()
{
	// RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvHandles_ = DescriptorManager::GetInstance()->GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetRTV(), DescriptorManager::GetInstance()->GetDescSize().RTV, 2);
	DirectXCommon::GetInstance()->GetDevice()->CreateRenderTargetView(texBuff_.Get(), &rtvDesc, rtvHandles_);
}

void PostProcess::CreateSRV()
{
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();

	// metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = WinApp::kWindowWidth; // Textureの幅
	resourceDesc.Height = WinApp::kWindowHeight; // Textureの高さ
	resourceDesc.MipLevels = 1; // mipmapの数
	resourceDesc.DepthOrArraySize = 1; // 奥行 or 配列Textureの配列数
	resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // TextureのFormat
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定 
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	// 利用するHeapの設定。非常に特殊な運用。02_04exで一般的なケース版がある
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM; // 細かい設定を行う
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // WriteBackポリシーでCPUアクセス可能
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0; // プロセッサの近くに配置
	D3D12_CLEAR_VALUE clearValue{};
	float clearColor[] = { 0.1f, 0.25f, 0.5f, 1.0f };
	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	clearValue.Color[0] = clearColor[0];
	clearValue.Color[1] = clearColor[1];
	clearValue.Color[2] = clearColor[2];
	clearValue.Color[3] = clearColor[3];
	// Resourceの作成
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定。特になし。
		&resourceDesc, // Resourceの設定
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue, // Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&texBuff_)); // 作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));

	SrvManager::GetInstance()->CreatePostProcessSrv(texBuff_.Get(), index_);

}

void PostProcess::CreateBuffer()
{
	// effectの種類で区別する
	if (type_ == Bloom) {
		bloom_ = CreateResource::CreateBufferResource(sizeof(BloomParam));
		bloom_->Map(0, nullptr, reinterpret_cast<void**>(&bloomData_));
		bloomData_->stepWidth = 0.001f;
		bloomData_->sigma = 0.005f;
		bloomData_->lightStrength = 1.0f;
		bloomData_->bloomThreshold = 0.2f;
	}
	else if (type_ == Vignette) {
		vignette_ = CreateResource::CreateBufferResource(sizeof(VignetteParam));
		vignette_->Map(0, nullptr, reinterpret_cast<void**>(&vignetteData_));
		vignetteData_->scale = 16.0f;
		vignetteData_->exponent = 0.8f;
	}
	else if (type_ == GaussianBlur) {
		gaussian_ = CreateResource::CreateBufferResource(sizeof(GaussianParam));
		gaussian_->Map(0, nullptr, reinterpret_cast<void**>(&gaussianData_));
		gaussianData_->sigma = 0.001f;
		gaussianData_->stepWidth = 0.005f;
	}
	else if (type_ == DepthOutline) {
		depthOutline_ = CreateResource::CreateBufferResource(sizeof(ProjectionInverse));
		depthOutline_->Map(0, nullptr, reinterpret_cast<void**>(&projection_));
		camera_.Initialize();
	}
	else if (type_ == RadialBlur) {
		radialBlur_ = CreateResource::CreateBufferResource(sizeof(RadialParam));
		radialBlur_->Map(0, nullptr, reinterpret_cast<void**>(&radialData_));
		radialData_->center = Vector2(0.5f, 0.5f);
		radialData_->blurWidth = 0.01f;
	}
	else if (type_ == Dissolve) {
		dissolve_ = CreateResource::CreateBufferResource(sizeof(DissolveParam));
		dissolve_->Map(0, nullptr, reinterpret_cast<void**>(&dissolveData_));
		dissolveData_->threshold = 0.5f;
	}
	else if (type_ == Random) {
		random_ = CreateResource::CreateBufferResource(sizeof(RandomParam));
		random_->Map(0, nullptr, reinterpret_cast<void**>(&randomData_));
		randomData_->time = 0.0f;
	}
}

void PostProcess::CreatePipeLine()
{
	if (type_ == Bloom) {
		pipeline_ = GraphicsPipeline::GetInstance()->GetPSO().Bloom;
	}
	else if (type_ == Grayscale) {
		pipeline_ = GraphicsPipeline::GetInstance()->GetPSO().Grayscale;
	}
	else if (type_ == Vignette) {
		pipeline_ = GraphicsPipeline::GetInstance()->GetPSO().Vignette;
	}
	else if (type_ == GaussianBlur) {
		pipeline_ = GraphicsPipeline::GetInstance()->GetPSO().GaussianBlur;
	}
	else if (type_ == LuminanceOutline) {
		pipeline_ = GraphicsPipeline::GetInstance()->GetPSO().LuminanceOutline;
	}
	else if (type_ == DepthOutline) {
		pipeline_ = GraphicsPipeline::GetInstance()->GetPSO().DepthOutline;
	}
	else if (type_ == RadialBlur) {
		pipeline_ = GraphicsPipeline::GetInstance()->GetPSO().RadialBlur;
	}
	else if (type_ == Dissolve) {
		pipeline_ = GraphicsPipeline::GetInstance()->GetPSO().Dissolve;
	}
	else if (type_ == Random) {
		pipeline_ = GraphicsPipeline::GetInstance()->GetPSO().Random;
	}
}

void PostProcess::SetConstantBuffer()
{
	if (type_ == Bloom) {
		DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, bloom_->GetGPUVirtualAddress());
	}
	else if (type_ == Vignette) {
		DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, vignette_->GetGPUVirtualAddress());
	}
	else if (type_ == GaussianBlur) {
		DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, gaussian_->GetGPUVirtualAddress());
	}
	else if (type_ == DepthOutline) {
		DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, depthOutline_->GetGPUVirtualAddress());
		DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(2, SrvManager::GetInstance()->GetGPUHandle(10));
	}
	else if (type_ == RadialBlur) {
		DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, radialBlur_->GetGPUVirtualAddress());
	}
	else if (type_ == Dissolve) {
		DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, dissolve_->GetGPUVirtualAddress());
		DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(2, SrvManager::GetInstance()->GetGPUHandle(maskTexHandle_));
	}
	else if (type_ == Random) {
		DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, random_->GetGPUVirtualAddress());
	}
}

void PostProcess::CreateDepthTextureSrv()
{
	SrvManager::GetInstance()->CreateDepthTextureSrv(DirectXCommon::GetInstance()->GetDepthBuffer(), 10);
}

void PostProcess::PreDepthBarrier()
{
	depthBarrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// Noneにしておく
	depthBarrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バリアを張る対象のリソース
	depthBarrier_.Transition.pResource = DirectXCommon::GetInstance()->GetDepthBuffer();
	// 遷移前（現在）のResourceState
	depthBarrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	// 遷移後のResourceState
	depthBarrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	DirectXCommon::GetCommandList()->ResourceBarrier(1, &depthBarrier_);
}

void PostProcess::PostDepthBarrier()
{
	depthBarrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	depthBarrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	DirectXCommon::GetCommandList()->ResourceBarrier(1, &depthBarrier_);
}

void PostProcess::PreDraw()
{

	barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// Noneにしておく
	barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バリアを張る対象のリソース
	barrier_.Transition.pResource = texBuff_.Get();
	// 遷移前（現在）のResourceState
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	// 遷移後のResourceState
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	DirectXCommon::GetCommandList()->ResourceBarrier(1, &barrier_);

	// discriptorのmanagerから持ってくる
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = DescriptorManager::GetInstance()->GetDSV()->GetCPUDescriptorHandleForHeapStart();
	// レンダーターゲットをセット
	DirectXCommon::GetCommandList()->OMSetRenderTargets(1, &rtvHandles_, false, &dsvHandle);
	// 全画面クリア
	float clearColor[] = { 0.1f, 0.25f, 0.5f, 1.0f };
	DirectXCommon::GetCommandList()->ClearRenderTargetView(rtvHandles_, clearColor, 0, nullptr);
	// 深度バッファのクリア
	DirectXCommon::GetCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	// viewport
	DirectXCommon::GetCommandList()->RSSetViewports(1, &viewport);
	// scissorRect
	DirectXCommon::GetCommandList()->RSSetScissorRects(1, &scissorRect);
	// heapのセット
	ID3D12DescriptorHeap* heaps[] = { DescriptorManager::GetInstance()->GetSRV() };
	DirectXCommon::GetCommandList()->SetDescriptorHeaps(_countof(heaps), heaps);
}

void PostProcess::PostDraw()
{
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	DirectXCommon::GetCommandList()->ResourceBarrier(1, &barrier_);
}

void PostProcess::Draw()
{
	if (type_ == DepthOutline) {
		PreDepthBarrier();
		projection_->projectionInverse = Inverse(camera_.matProjection);
	}

	// effectの種類によって変えてる
	CreatePipeLine();

	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(pipeline_.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(pipeline_.graphicsPipelineState_.Get()); // PSOを設定
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// srvの設定
	DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(0, SrvManager::GetInstance()->GetGPUHandle(index_));

	// effectの種類によって変えてる
	SetConstantBuffer();

	// 描画。(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	DirectXCommon::GetCommandList()->DrawInstanced(3, 1, 0, 0);

	if (type_ == DepthOutline) {
		PostDepthBarrier();
	}

}

void PostProcess::SetEffect(PostEffectType type)
{
	type_ = type;
	// depthoutlineならそれ用のsrvも作る
	if (type_ == DepthOutline) {
		CreateDepthTextureSrv();
	}

	CreateBuffer();
}
