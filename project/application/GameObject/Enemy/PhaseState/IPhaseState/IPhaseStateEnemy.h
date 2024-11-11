#pragma once

class Player;
class Enemy;

class IPhaseStateEnemy {
public:

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IPhaseStateEnemy();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="pEnemy"></param>
	virtual void Update(Enemy* pEnemy) = 0;

	/// <summary>
	/// playerのポインタを設定する
	/// </summary>
	/// <param name="player"></param>
	virtual void SetPlayer(Player* player) { player_ = player; };

protected:
	// playerのポインタを借りる
	Player* player_ = nullptr;
};
