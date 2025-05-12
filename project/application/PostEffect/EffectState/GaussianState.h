/**
* @file GaussianState.h
* @brief ガウシアンブラー
* @author 仁平 琉乃
*/

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

