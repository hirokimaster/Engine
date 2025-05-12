/**
* @file GaussianState.cpp
* @brief ガウシアンブラー
* @author 仁平 琉乃
*/

#include "GaussianState.h"

void GaussianState::Apply(PostProcess* pPostProcess, PostEffect* pPostEffect)
{
	pPostProcess->SetEffect(PostEffectType::GaussianBlur);
	pPostEffect;
}
