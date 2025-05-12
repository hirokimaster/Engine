/**
* @file PlayerDamageState.h
* @brief playerがダメージを食らったとき(ビネット)
* @author 仁平 琉乃
*/

#pragma once
#include "application/PostEffect/IPostEffect.h"
#include "application/PostEffect/EffectState/NoneEffectState.h"

class PlayerDamegeState : public IPostEffect {
public:
	/// <summary>
	/// 適用
	/// </summary>
	void Apply(PostProcess* pPostProcess, PostEffect* pPostEffect)override;

private:
	float effectTime_ = 30.0f; // エフェクト用のタイマー

};

