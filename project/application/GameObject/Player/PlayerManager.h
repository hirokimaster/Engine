#pragma once
#include "application/GameObject/Player/Player.h"
#include "engine/TextureManager/TextureManager.h"

/// <summary>
/// playerでする処理はここにまとめて書いてシーンで呼び出す
/// </summary>
class PlayerManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(Camera& camera);

private:
	std::unique_ptr<Object3DPlacer>	objectPlayer_ = nullptr;
	std::unique_ptr<Player> player_ = nullptr;
	uint32_t texHandlePlayer_ = 0;
};

