#pragma once
#include "engine/Graphics/PostProcess/PostProcess.h"

class PostEffect;

class IPostEffect
{
public:
	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IPostEffect() = default;

	/// <summary>
	/// エフェクト適用
	/// </summary>
	virtual void Apply(PostProcess* pPostProcess, PostEffect* pPostEffect) = 0;
};

