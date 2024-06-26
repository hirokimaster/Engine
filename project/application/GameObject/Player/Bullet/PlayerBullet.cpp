#include "PlayerBullet.h"

void PlayerBullet::Initialize(Object3DPlacer* object, uint32_t texHandle, const std::string& model)
{
	BaseObject::Initialize(object, texHandle, model); // 共通処理初期化
}

void PlayerBullet::Update()
{
	//移動
	worldTransform_.translate = worldTransform_.translate + velocity_;
	BaseObject::Update();  // 共通の更新処理
}

void PlayerBullet::Draw(Camera& camera)
{
	BaseObject::Draw(camera); // 共通の描画処理
}
