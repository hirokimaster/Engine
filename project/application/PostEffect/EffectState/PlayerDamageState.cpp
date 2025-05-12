/**
* @file PlayerDamageState.cpp
* @brief playerがダメージを食らったとき(ビネット)
* @author 仁平 琉乃
*/

#include "PlayerDamageState.h"
#include "application/PostEffect/PostEffect.h"

void PlayerDamegeState::Apply(PostProcess* pPostProcess, PostEffect* pPostEffect)
{
	// effectTimerが0になるまでにビネットをかける
	--effectTime_;
	if (effectTime_ > 0.0f) {

		VignetteParam param = {
			.scale = 60.0f,
			.exponent = 0.3f
		};

		pPostProcess->SetEffect(PostEffectType::Vignette);
		pPostProcess->SetVignetteParam(param);
	}

	// ダメージエフェクト終わり
	if (effectTime_ < 0.0f) {
	   // エフェクトをなしにする
	   pPostEffect->ChangeState(std::make_unique<NoneEffectState>());
	}

}
