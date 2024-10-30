/**
* @file EnemyStateSortie.cpp
* @brief 敵の生成、挙動
* @author 仁平 琉乃
*/

#include "EnemyStateSortie.h"
#include "application/GameObject/Enemy/Enemy.h"

EnemyStateSortie::~EnemyStateSortie()
{
}

void EnemyStateSortie::Update(Enemy* pEnemy)
{
	// 速度ベクトル
	Vector3 velocity = { 0.0f,0.0f,0.0f };
	const float kMoveSpeed = 1.0f;

	// プレイヤーに向かって移動する
	Vector3 diff = player_->GetWorldPosition() - pEnemy->GetWorldPosition(); // プレイヤーとの差分ベクトルを求める
	Vector3 distanceLimit = { 2.0f,2.0f,2.0f }; // この距離をこえたら向かうのをやめる
	if (Length(diff) >= Length(distanceLimit) && isFollow_) {
		diff = Normalize(diff); // 正規化
		velocity = Normalize(diff); // 正規化
		velocity = kMoveSpeed * diff; // 差分ベクトルに速度をかける
		pEnemy->SetVelocity(velocity);
	}
	else {
		isFollow_ = false;
		velocity = { 0.0f,0.0f,kMoveSpeed };
		pEnemy->SetVelocity(velocity);
	}
	// 移動
	pEnemy->Move();
}