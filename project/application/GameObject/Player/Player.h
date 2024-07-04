#pragma once
#include "application/GameObject/BaseObject/BaseObject.h"
#include "application/GameObject/Player/Bullet/PlayerBullet.h"
#include "engine/TextureManager/TextureManager.h"
#include "engine/Utility/CollisionManager/Collider/Collider.h"

class Player : public BaseObject, public Collider 
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

private: // クラス内でしか使わない

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

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision()override;

public:

#pragma region getter

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() const { return bullets_; }

	Vector3 GetWorldPosition()override;

#pragma endregion

#pragma region setter

	void SetParent(const WorldTransform* parent) { worldTransform_.parent = parent; }

	void SetPosition(Vector3 position) { worldTransform_.translate = position; }

#pragma endregion

private:
	const float kMoveSpeed_ = 0.2f; // 移動スピード
	std::list<std::unique_ptr<PlayerBullet>> bullets_; // 弾のリスト
	std::list<std::unique_ptr<Object3DPlacer>> objectBullets_;
	std::list<std::unique_ptr<PlayerBullet>>::iterator bulletsItr_; // 弾のイテレータ
	uint32_t texHandleBullet_ = 0;
};
