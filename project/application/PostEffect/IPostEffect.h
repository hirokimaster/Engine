/**
* @file IPostEffect.h
* @brief ポストエフェクトの基底クラス
* @author 仁平 琉乃
*/

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

