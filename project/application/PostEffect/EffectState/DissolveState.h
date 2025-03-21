#pragma once
#include "application/PostEffect/IPostEffect.h"
#include "engine/2d/Sprite/Sprite.h"

class DissolveState : public IPostEffect {
public:

	/// <summary>
	/// 適用
	/// </summary>
	/// <param name="pPostProcess"></param>
	/// <param name="pPostEffect"></param>
	void Apply(PostProcess* pPostProcess, PostEffect* pPostEffect)override;
	
private:
	DissolveParam param_{ .threshold = 0.0f };
	bool flag_ = true;
};
