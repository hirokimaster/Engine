#include "NoneEffectState.h"
#include "application/PostEffect/PostEffect.h"

void NoneEffectState::Apply(PostProcess* pPostProcess, PostEffect* pPostEffect)
{
	// 何も適用しない
	pPostProcess->SetEffect(PostEffectType::Bloom);
	pPostEffect;
}
