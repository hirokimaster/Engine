#include "AdjustmentVariables.h"

AdjustmentVariables* AdjustmentVariables::GetInstance()
{
	static AdjustmentVariables instance;
	return &instance;
}

void AdjustmentVariables::CreateGroup(const string& groupName)
{
	// 指定名のオブジェクトが無ければ追加する
	datas_[groupName];
}

void AdjustmentVariables::Update()
{
	if (!ImGui::Begin("Adjustment Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}

	if (!ImGui::BeginMenuBar()) {
		// 各グループについて
		for (auto& groups : datas_) {
			// グループ名を取得
			const string& groupName = groups.first;
			// グループの参照を取得
			Group& group = groups.second;

			if (!ImGui::BeginMenu(groupName.c_str())) {
				continue;

				// 各項目について
				for (auto& Items : group) {
					// 項目名を取得
					const string& itemName = Items.first;
					// 項目の参照を取得
					Item& item = Items.second;

					// int32_t型
					if (std::holds_alternative<int32_t>(item)) {
						int32_t* ptr = std::get_if<int32_t>(&item);
						ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
					}
					// float型
					else if (std::holds_alternative<float>(item)) {
						float* ptr = std::get_if<float>(&item);
						ImGui::SliderFloat(itemName.c_str(), ptr, 0.0f, 100.0f);
					}
					// Vector3型
					else if (std::holds_alternative<Vector3>(item)) {
						Vector3* ptr = std::get_if<Vector3>(&item);
						ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
					}
				}
			}
		}

		return;
	}

}
