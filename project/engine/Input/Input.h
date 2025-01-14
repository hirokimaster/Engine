/**
* @file Input.h
* @brief キーボード、コントローラの入力
* @author 仁平 琉乃
*/

#pragma once
#include"engine/Base/WinApp/WinApp.h"
#include <cassert>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Xinput.h>
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"dinput8.lib")
#include <cmath>

class Input {
public:
	/// <summary>
	/// シングルトンインスタンス取得
	/// </summary>
	/// <returns></returns>
	static Input* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	static void Update();

#pragma region キーボード

	/// <summary>
	/// 押してるとき
	/// </summary>
	/// <param name="keyNum"></param>
	/// <returns></returns>
	bool PushKey(uint8_t keyNum);

	/// <summary>
	/// 押したとき
	/// </summary>
	/// <param name="keyNum"></param>
	/// <returns></returns>
	bool PressedKey(uint32_t keyNum);

#pragma endregion

	
#pragma region コントローラー

	/// <summary>
	/// ジョイスティックの状態を取得
	/// </summary>
	/// <returns></returns>
	static bool GetJoystickState();

	/// <summary>
	/// ジョイスティックの状態を取得
	/// </summary>
	/// <param name="out"></param>
	/// <returns></returns>
	bool GetJoystickState(XINPUT_STATE& out);

	/// <summary>
	///  押されたとき
	/// </summary>
	/// <param name="button"></param>
	/// <returns></returns>
	bool PressedButton(WORD button);

	/// <summary>
	/// 押したとき
	/// </summary>
	/// <param name="button"></param>
	/// <returns></returns>
	bool PushButton(WORD button);

	/// <summary>
	/// deadzoneの設定
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	//void ApplyDeadZone(SHORT& x, SHORT& y);

#pragma region setter

	//void SetDeadZone(float deadZone) { deadZone_ = deadZone; }

#pragma endregion

#pragma endregion

private:

	IDirectInput8* directInput = nullptr;
	IDirectInputDevice8* keyboard = nullptr;

	BYTE keys[256] = {};
	BYTE preKeys[256] = {};

	bool isInitialize = false;

	XINPUT_STATE state_{};
	XINPUT_STATE preState_{};

	// 初期値は20%くらい
	//float deadZone_ = SHRT_MAX * 0.8f;

private:

	Input() = default;
	~Input() = default;
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;
};