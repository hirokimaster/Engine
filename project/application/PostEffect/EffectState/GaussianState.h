#pragma once
#pragma once
#include "application/PostEffect/IPostEffect.h"
#include "application/PostEffect/EffectState/NoneEffectState.h"

class GaussianState : public IPostEffect
{
	/// <summary>
	/// 適用
	/// </summary>
	void Apply(PostProcess* pPostProcess, PostEffect* pPostEffect)override;
};

