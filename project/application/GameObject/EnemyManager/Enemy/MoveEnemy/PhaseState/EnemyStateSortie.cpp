/**
* @file EnemyStateSortie.cpp
* @brief 敵の生成、挙動
* @author 仁平 琉乃
*/

#include "EnemyStateSortie.h"
#include "application/GameObject/EnemyManager/Enemy/MoveEnemy/MoveEnemy.h"
#include "application/GameObject/Player/Player.h"

EnemyStateSortie::~EnemyStateSortie()
{
}

void EnemyStateSortie::Update(MoveEnemy* pEnemy)
{
	if (player_->GetWorldPosition().z >= pEnemy->GetEventTrigger().z) {
		isSortie_ = true;
		pEnemy->SetIsSortie(isSortie_);
	}
	// ポジションまで移動したら攻撃モードに移る
	if (isSortie_) {
		if (moveParam_ <= 1.0f) {
			moveParam_ += 1.0f / 60.0f;
			// 現在の位置と少し先の位置
			Vector3 currentPosition = CatmullRomPosition(pEnemy->GetMoveControlPoints(), moveParam_);
			Vector3 nextPosition = CatmullRomPosition(pEnemy->GetMoveControlPoints(), std::min(moveParam_ + 0.01f, 1.0f));
			// 進行方向
			Vector3 direction = nextPosition - currentPosition;
			float length = Length(direction);
			// 
			if (length > 0.0001f) {
				// 正規化
				direction = Normalize(direction);
				// y軸回転
				float yaw = std::atan2(direction.x, direction.z);
				// x軸回転
				float pitch = std::asin(direction.y);
				// z軸回転使わないゼロに
				Vector3 rotate = Vector3(pitch, yaw, 0.0f);
				pEnemy->SetRotate(rotate);
			}
			// 位置を更新
			move_ = currentPosition;
			pEnemy->SetPosition(move_);
		}
		else {
			moveParam_ = 1.0f;
			isSortie_ = false;
			pEnemy->ChangeState(std::make_unique<EnemyStateFire>());
		}
	}

	
}