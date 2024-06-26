#include "Player.h"

void Player::Initialize(Object3DPlacer* object, uint32_t texHandle, const std::string& model)
{
	BaseCharacter::Initialize(object, texHandle, model); // 共通処理初期化
}

void Player::Update()
{
	BaseCharacter::Update(); // 共通の更新処理
}

void Player::Draw(Camera& camera)
{
	BaseCharacter::Draw(camera); // 共通の描画処理
}
