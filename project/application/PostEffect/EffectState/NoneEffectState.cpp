/**
* @file NoneEffectState.cpp
* @brief エフェクトなし
* @author 仁平 琉乃
*/

#include "NoneEffectState.h"
#include "application/PostEffect/PostEffect.h"

void NoneEffectState::Apply(PostProcess* pPostProcess, PostEffect* pPostEffect)
{
	// 何も適用しない
	pPostProcess->SetEffect(PostEffectType::None);
	pPostEffect;
}
