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
