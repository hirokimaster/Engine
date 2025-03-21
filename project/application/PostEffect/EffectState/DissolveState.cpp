#include "DissolveState.h"

void DissolveState::Apply(PostProcess* pPostProcess, PostEffect* pPostEffect)
{
	// dissolve適用
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
}
