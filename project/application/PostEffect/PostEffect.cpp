/**
* @file PostEffect.cpp
* @brief ポストエフェクト切り替え
* @author 仁平 琉乃
*/

#include "PostEffect.h"


void PostEffect::Initialize()
{
	postProcess_ = std::make_unique<PostProcess>();
	postProcess_->Initialize();
	postProcess_->SetEffect(PostEffectType::None);
	// 初期はエフェクトなし
	effectState_ = std::make_unique<NoneEffectState>();
}

void PostEffect::ChangeState(std::unique_ptr<IPostEffect> newState)
{
	effectState_ = std::move(newState);	
}

void PostEffect::Update()
{
	// 適用
	effectState_->Apply(postProcess_.get(), this);
}
