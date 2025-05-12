/**
* @file DissolveState.cpp
* @brief ディゾルブ
* @author 仁平 琉乃
*/

#include "DissolveState.h"

void DissolveState::Apply(PostProcess* pPostProcess, PostEffect* pPostEffect)
{
	// マスク設定
	pPostProcess->SetMaskTexture(TextureManager::GetTexHandle("TempTexture/noise0.png"));

	// 変化させる
	if (flag_) {
		pPostProcess->SetDissolveParam(param_);
		param_.threshold += 0.02f;

		if (param_.threshold >= 1.0f) {
			flag_ = false;
		}
	}
	else {

		pPostProcess->SetDissolveParam(param_);
		param_.threshold -= 0.02f;

		if (param_.threshold <= 0.0f) {
			flag_ = true;
		}
	}

	pPostEffect;
}
