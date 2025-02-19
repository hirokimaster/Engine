#pragma once
#include "application/PostEffect/IPostEffect.h"
#include "application/PostEffect/EffectState/NoneEffectState.h"
#include "application/PostEffect/EffectState/PlayerDamageState.h"


class PostEffect {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 状態を変える
	/// </summary>
	/// <param name=""></param>
	void ChangeState(std::unique_ptr<IPostEffect> newState);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

#pragma region Getter

	PostProcess* GetPostProcess() { return postProcess_.get(); }

#pragma endregion

private:
	std::unique_ptr<PostProcess> postProcess_ = nullptr; // ポストプロセス
	std::unique_ptr<IPostEffect> effectState_ = nullptr;
};