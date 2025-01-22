#include "Laser.h"

void Laser::Initialize()
{
	// 共通部分の初期化
	IGimmick::Initialize();
	object_->SetModel("LevelEditorObj/laser.obj");
	texHandle_ = TextureManager::Load("resources/TempTexture/mount.jpg");
	object_->SetWorldTransform(worldTransform_);
	object_->SetTexHandle(texHandle_);
}

void Laser::Update()
{
	// 共通部分の更新
	IGimmick::Update();
}

void Laser::Draw(Camera& camera)
{
	IGimmick::Draw(camera);
}

Vector3 Laser::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}

void Laser::OnCollision()
{
	// 当たった
	isHit_ = true;
}
