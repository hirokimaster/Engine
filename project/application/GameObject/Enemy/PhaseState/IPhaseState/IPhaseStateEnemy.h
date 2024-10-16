#pragma once

class Enemy;

class IPhaseStateEnemy {
public:

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="pEnemy"></param>
	virtual void Update(Enemy* pEnemy) = 0;
};
