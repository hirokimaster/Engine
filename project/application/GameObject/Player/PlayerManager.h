#pragma once
#include "application/GameObject/Player/Player.h"
#include "engine/TextureManager/TextureManager.h"
#include"engine/Utility/CollisionManager/CollisionManager.h"

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
	void Update(const Camera& camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(Camera& camera);

	/// <summary>
	/// colliderにpushする
	/// </summary>
	void ColliderPush(CollisionManager* collision);

#pragma region getter

	Vector3 GetPlayerPosition() { return player_->GetWorldPosition(); }

#pragma endregion

#pragma region setter

	void SetParent(const WorldTransform* parent) { player_->SetParent(parent); }

#pragma endregion

private:
	std::unique_ptr<Object3DPlacer>	objectPlayer_ = nullptr;
	std::unique_ptr<Player> player_ = nullptr;
	uint32_t texHandlePlayer_ = 0;
};

