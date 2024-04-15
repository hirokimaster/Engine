#pragma once
#include "engine/Base/DX/DirectXCommon.h"
#include "engine/Utility/StringUtility.h"
#include "engine/Utility/ShaderCompile.h"
#include "engine/CreateResource/CreateResource.h"
#include "engine/GraphicsPipeline/GraphicsPipeline.h"
#include "engine/Math/Mathfunction.h"
#include "engine/Transform/WorldTransform.h"
#include "engine/Camera/Camera.h"
#include "engine/TextureManager/TextureManager.h"
#include "engine/Math/Vector2.h"
		  
class Sprite {
public: // メンバ関数

	Sprite() = default;
	~Sprite();

	static void StaticInitialize();

	static void StaticUpdate();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(uint32_t texHandle);

	/// <summary>
	/// 頂点データ作成
	/// </summary>
	void CreateVertex();

	/// <summary>
	/// スプライト生成
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	static Sprite* Create(uint32_t texHandle, Vector2 position = {0,0}, Vector4 color = {1,1,1,1});

#pragma region Getter
	// 座標の取得
	const Vector2& GetPosition() const { return position_; }
	// アンカーポイント取得
	const Vector2& GetAnchorPoint() const { return anchorPoint_; }
	// テクスチャ座標取得
	const Vector2& GetTextureLeftTop() const { return textureLeftTop_; }
	// テクスチャ切り出しサイズ取得
	const Vector2& GetTextureSize() const { return textureSize_; }

#pragma endregion 

#pragma region Setter
	// 座標の設定
	void SetPosition(const Vector2& position) { position_ = position; }
	// 色の設定
	void SetColor(const Vector4& color) { *materialData_ = color; }
	// アンカーポイントの設定
	void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }
	// テクスチャ座標設定
	void SetTextureLeftTop(const Vector2& textureLeftTop) { textureLeftTop_ = textureLeftTop; }
	// テクスチャ切り出しサイズ設定
	void SetTextureSize(const Vector2& textureSize) { textureSize_ = textureSize; }
	// テクスチャハンドルの設定
	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }
	// サイズの設定
	void SetSize(const Vector2& size) { size_ = size; }
#pragma endregion

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="v"></param>
	/// <param name="t"></param>
	void Draw();

private: // メンバ変数

	static Camera camera_;
	D3D12_VERTEX_BUFFER_VIEW VBV_{};
	D3D12_INDEX_BUFFER_VIEW IBV_{};
	Resource resource_ = {};
	WorldTransform worldTransform_ = {};
	Vector2 position_ = {};
	Vector2 size_{};
	Vector4* materialData_ = nullptr;
	VertexData* vertexDataSprite_ = nullptr;
	Vector2 anchorPoint_ = {};
	Vector2 textureLeftTop_ = {};
	Vector2 textureSize_ = {};
	uint32_t texHandle_ = 0;

private:

	void AdjustTextureSize(uint32_t texHandle);
};
