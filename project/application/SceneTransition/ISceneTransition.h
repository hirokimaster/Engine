/**
* @file ISceneTransition.h
* @brief 遷移クラスのインターフェース
*/
#pragma once
#include <functional>

class ISceneTransition {
public:

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~ISceneTransition() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 開始
	/// </summary>
	/// <param name="call"></param>
	virtual void Start(std::function<void()> call) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

};