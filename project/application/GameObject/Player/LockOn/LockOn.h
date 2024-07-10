#pragma once
#include "application/GameObject/Enemy/Enemy.h"

class LockOn {
public:

	void Initialize();

	void Update();

	void Draw();

private:
	// ロックオン対象
	const Enemy* target_ = nullptr;
};