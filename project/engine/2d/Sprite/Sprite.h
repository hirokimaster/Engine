/**
* @file Sprite.h
* @brief スプライトの初期化、生成
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Base/DX/DirectXCommon.h"
#include "engine/Graphics/CreateResource/CreateResource.h"
#include "engine/Graphics/Pipeline/GraphicsPipeline.h"
#include "engine/Math/Mathfunction.h"
#include "engine/Graphics/Transform/WorldTransform.h"
#include "engine/Graphics/Camera/Camera.h"
#include "engine/Math/Vector2.h"
#include "engine/Graphics/TextureManager/TextureManager.h"
		  
class Sprite {
public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Sprite();

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// 静的更新処理
	/// </summary>
	static void StaticUpdate();

	/// <summary>
	/// スプライト生成
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	static Sprite* Create(uint32_t texHandle, const Vector2& position = {0,0}, const Vector4& color = {1,1,1,1});

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="v"></param>
	/// <param name="t"></param>
	void Draw();

	void AdjustTextureSize(uint32_t texHandle);

private:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(uint32_t texHandle);

	/// <summary>
	/// 頂点データ作成
	/// </summary>
	void CreateVertex();

public:

#pragma region Getter
	// 座標の取得
	const Vector2& GetPosition() const { return position_; }
	// アンカーポイント取得
	const Vector2& GetAnchorPoint() const { return anchorPoint_; }
	// テクスチャ座標取得
	const Vector2& GetTextureLeftTop() const { return textureLeftTop_; }
	// テクスチャ切り出しサイズ取得
	const Vector2& GetTextureSize() const { return textureSize_; }
	// scale取得
	const Vector2& GetScale()const { return scale_; }

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
	// スケールの変更
	void SetScale(const Vector2& scale) { scale_ = scale; }
#pragma endregion

private: // メンバ変数

	static Camera camera_;
	D3D12_VERTEX_BUFFER_VIEW VBV_{};
	D3D12_INDEX_BUFFER_VIEW IBV_{};
	Resource resource_ = {};
	WorldTransform worldTransform_ = {};
	Vector2 position_ = {};
	Vector2 scale_ = { 1.0f,1.0f };
	Vector2 size_{};
	Vector4* materialData_ = nullptr;
	VertexData* vertexDataSprite_ = nullptr;
	Vector2 anchorPoint_ = {};
	Vector2 textureLeftTop_ = {};
	Vector2 textureSize_ = {};
	uint32_t texHandle_ = 0;
};
