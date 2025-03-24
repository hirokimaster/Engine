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
