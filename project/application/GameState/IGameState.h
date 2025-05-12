/**
* @file IGameState.h
* @brief ゲームの状態の基底クラス(例えばゲームオーバーとかスタートとか)
* @author 仁平 琉乃
*/

#pragma once

class IGameState {
public:
	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IGameState() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

private:

};
