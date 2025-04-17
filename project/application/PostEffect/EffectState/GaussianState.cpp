#include "GaussianState.h"

void GaussianState::Apply(PostProcess* pPostProcess, PostEffect* pPostEffect)
{
	pPostProcess->SetEffect(PostEffectType::GaussianBlur);
	pPostEffect;
}
