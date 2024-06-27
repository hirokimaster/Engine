#pragma once
#include "application/GameObject/BaseObject/BaseObject.h"
#include "application/GameObject/Player/Bullet/PlayerBullet.h"
#include "engine/TextureManager/TextureManager.h"

class Player : public BaseObject
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="object"></param>
	/// <param name="texHandle"></param>
	/// <param name="model"></param>
	void Initialize(Object3DPlacer* object, uint32_t texHandle, const std::string& model)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(Camera& camera)override;

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();

	/// <summary>
	/// 弾の更新処理
	/// </summary>
	void UpdateBullet();

private:
	const float kMoveSpeed_ = 0.2f; // 移動スピード
	std::list<std::unique_ptr<PlayerBullet>> bullets_; // 弾のリスト
	std::list<std::unique_ptr<Object3DPlacer>> bulletObjects_;
	std::list<std::unique_ptr<PlayerBullet>>::iterator bulletsItr_; // 弾のイテレータ
	std::list<std::unique_ptr<Object3DPlacer>>::iterator bulletObjectsItr_;
	uint32_t texHandleBullet_ = 0;
};
