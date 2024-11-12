/**
* @file ColliderConfig.h
* @brief 衝突判定の属性つける
* @author 仁平 琉乃
*/

#pragma once
#include <cstdint>

const uint32_t kCollisionAttributePlayer = 0b1;

const uint32_t kCollisionAttributeEnemy = 0b1 << 1;

const uint32_t kCollisionAttributeEnemyBullet = 0b1 << 2;

const uint32_t kCollisionAttributePlayerBullet = 0b1 << 3;
