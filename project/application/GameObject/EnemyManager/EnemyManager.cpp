/**
* @file EnemyManager.cpp
* @brief 敵の一括管理
* @author 仁平 琉乃
*/

#include "EnemyManager.h"

void EnemyManager::Initialize()
{
    // 動く敵
    auto it = loader_->GetObjectDatas().find("moveEnemy");
    if (it != loader_->GetObjectDatas().end()) {
        for (auto& objectData : it->second) {
            auto enemy = std::make_unique<MoveEnemy>();
            enemy->Initialize();
            enemy->SetPlayer(player_);
            enemy->SetPosition(objectData.controlPoint.front()); // 初期位置は最初の制御点
            enemy->SetEventNum(objectData.eventNum);
            // 移動ルートの制御点を設定
            for (const auto& point : objectData.controlPoint) {
                enemy->SetMoveControlPoints(point);
            }

            // イベントトリガー
            auto eventIt = loader_->GetObjectDatas().find("EventTrigger");
            if (eventIt != loader_->GetObjectDatas().end()) {
                for (auto& event : eventIt->second) {
                    if (objectData.eventNum == event.eventNum) {
                        enemy->SetEventTrigger(event.translate);
                    }
                }
            }

            enemys_.push_back(std::move(enemy));
        }
    }

    // 固定の敵
    auto it2 = loader_->GetObjectDatas().find("fixedEnemy");
    if (it2 != loader_->GetObjectDatas().end()) {
        for (auto& objectData : it2->second) {
            auto enemy = std::make_unique<FixedEnemy>();
            enemy->Initialize();
            enemy->SetPlayer(player_);
            enemy->SetPosition(objectData.translate); // 初期位置は最初の制御点
            enemy->SetScale(objectData.scale);
            enemy->SetBulletObjectPool(bulletObjectPool_);
            enemys_.push_back(std::move(enemy));
        }
    }
}

void EnemyManager::Update()
{
	for (auto& enemy : enemys_) {
		enemy->Update();
	}

    // デスフラグが呼ばれたら削除
    enemys_.remove_if([](std::unique_ptr<IEnemy>& enemy) {
        if (enemy->GetIsDead()) {

            return true;
        }
        return false;
        });
}