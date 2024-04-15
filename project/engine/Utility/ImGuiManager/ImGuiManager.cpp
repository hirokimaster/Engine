#include "ImGuiManager.h"

#ifdef _DEBUG
#include "engine/Base/DX/DirectXCommon.h"
#include "engine/Base/WinApp/WinApp.h"
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>
#endif

ImGuiManager* ImGuiManager::GetInstance(){
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Initialize(WinApp* winApp, DirectXCommon* dxCommon){
#ifdef _DEBUG
	dxCommon_ = dxCommon;
	IMGUI_CHECKVERSION();
	// ImGuiのコンテキストを生成
	ImGui::CreateContext();
	// ImGuiのスタイルを設定
	ImGui::StyleColorsDark();
	// プラットフォームとレンダラーのバックエンドを設定する
	ImGui_ImplWin32_Init(winApp->GetHwnd());
	ImGui_ImplDX12_Init(
		dxCommon_->GetDevice(), static_cast<int>(dxCommon_->GetBufferCount().BufferCount),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DescriptorManager::GetInstance()->GetSRV(),
		DescriptorManager::GetInstance()->GetSRV()->GetCPUDescriptorHandleForHeapStart(),
		DescriptorManager::GetInstance()->GetSRV()->GetGPUDescriptorHandleForHeapStart());
#endif

}

void ImGuiManager::Finalize(){
#ifdef _DEBUG
	
	// 後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

#endif
}

void ImGuiManager::Begin(){
#ifdef _DEBUG
	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif
}

void ImGuiManager::End(){
#ifdef _DEBUG
	ImGui::Render();
#endif
}

void ImGuiManager::Draw(){
#ifdef _DEBUG

	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { DescriptorManager::GetInstance()->GetSRV()};
	DirectXCommon::GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectXCommon::GetCommandList());
#endif
}


