/**
* @file NoneEffectState.h
* @brief エフェクトなし
* @author 仁平 琉乃
*/

#pragma once
#include "application/PostEffect/IPostEffect.h"

class NoneEffectState :public IPostEffect
{
public:
	/// <summary>
	/// 適用
	/// </summary>
	/// <param name="pPostProcess"></param>
	/// <param name="pPostEffect"></param>
	void Apply(PostProcess* pPostProcess, PostEffect* pPostEffect)override;

private:

};

