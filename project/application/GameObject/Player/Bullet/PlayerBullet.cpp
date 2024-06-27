#include "PlayerBullet.h"

void PlayerBullet::Initialize(Object3DPlacer* object, uint32_t texHandle, const std::string& model)
{
	BaseObject::Initialize(object, texHandle, model); // 共通処理初期化
}

void PlayerBullet::Update()
{
	Move(); // 移動
	BulletErase(); // 弾を削除
	BaseObject::Update();  // 共通の更新処理
}

void PlayerBullet::Draw(Camera& camera)
{
	BaseObject::Draw(camera); // 共通の描画処理
}

void PlayerBullet::Move()
{
	//移動
	worldTransform_.translate = worldTransform_.translate + velocity_;
}

void PlayerBullet::BulletErase()
{
	// 時間で消滅
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}
