#pragma once

class Player;
class MoveEnemy;

class BasePhaseStateEnemy {
public:

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~BasePhaseStateEnemy();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="pEnemy"></param>
	virtual void Update(MoveEnemy* pEnemy) = 0;

	/// <summary>
	/// playerのポインタを設定する
	/// </summary>
	/// <param name="player"></param>
	virtual void SetPlayer(Player* player) { player_ = player; };

protected:
	// playerのポインタを借りる
	Player* player_ = nullptr;
};
