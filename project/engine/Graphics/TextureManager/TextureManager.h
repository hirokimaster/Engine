/**
* @file TextureManager.h
* @brief テクスチャの読み込み、管理を行うクラス
* @author 仁平 琉乃
*/

#pragma once
#include <array>
#include <d3dx12.h>
#include <string>
#include <unordered_map>
#include <wrl.h>
#include <DirectXTex.h>
#include "engine/Utility/StringUtility.h"
#include "engine/Base/DX/DirectXCommon.h"
#include "engine/Graphics/DescriptorManager/SRVManager/SrvManager.h"
#include <iostream>
#include <vector>

class TextureManager {
public:
	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// 読み込み
	/// </summary>
	static void Load(const std::string& fileName);

#pragma region getter

	const DirectX::TexMetadata& GetMetaData(uint32_t textureIndex);

	/// <summary>
	/// mapからtextureHandleを取り出す
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	static uint32_t GetTexHandle(const std::string& name) {
		std::string path = "resources/" + name;
		auto it = TextureManager::GetInstance()->fileHandleMap.find(path);
		if (it == TextureManager::GetInstance()->fileHandleMap.end()) {
			// ファイル名が存在しない場合
			assert(0);
		}
		return it->second;
	}

#pragma endregion

private:
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

private: // クラス内で使う関数

	/// <summary>
	/// texture読み込み
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	static DirectX::ScratchImage LoadTexture(const std::string& filePath);

	/// <summary>
	/// texture読み込み
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="index"></param>
	static void  LoadTexture(const std::string& filePath, uint32_t index);

	/// <summary>
	/// textureResource作成
	/// </summary>
	/// <param name="metadata"></param>
	/// <returns></returns>
	static Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	/// <summary>
	/// textureDataのアップロード
	/// </summary>
	/// <param name="texture"></param>
	/// <param name="mipImages"></param>
	/// <returns></returns>
	[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);


private: // メンバ変数

	Microsoft::WRL::ComPtr<ID3D12Resource>texResource[1024];
	std::unordered_map<std::string, uint32_t> fileHandleMap;
	DirectX::TexMetadata metadata_[1024];
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource_[1024];
};
