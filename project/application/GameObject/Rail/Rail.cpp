/**
* @file Rail.cpp
* @brief レールを生成
* @author 仁平 琉乃
*/

#include "Rail.h"

void Rail::Update()
{
	Vector3 move{};
	t_ += 1.0f / 600.0f;
	move = CatmullRomPosition(controlPoints_, t_);
	player_->SetPosition(move);
}

