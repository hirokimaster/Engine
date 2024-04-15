#pragma once
#include <string>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <cassert>
#include <mutex>
#include <set>
#include <array>
#include <unordered_map>
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <wrl.h>

class Audio {
public:	// 構造体
	struct SoundData {
		// 波形フォーマット
		WAVEFORMATEX wfex;
		// バッファ
		BYTE* pBuffer;
		// バッファのサイズ
		unsigned int bufferSize;
	};

	struct ChunkHeader {
		char id[4];
		int32_t size;
	};

	struct RiffHeader {
		ChunkHeader chunk;
		char type[4];
	};

	struct FormatChunk {
		ChunkHeader chunk;
		WAVEFORMATEX fmt;
	};

public: // メンバ関数
	// 読み込みの最大数
	static const int kMaxSoundData = 256;

	/// <summary>
	///  シングルトンインスタンスの生成
	/// </summary>
	/// <returns></returns>
	static Audio* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();

	/// <summary>
	/// サウンドデータのロード
	/// </summary>
	/// <param name="filename"></param>
	/// <returns></returns>
	uint32_t SoundLoadWave(const char* filename);

	/// <summary>
	/// サウンドデータ破棄
	/// </summary>
	void SoundUnload();

	/// <summary>
	/// 再生
	/// </summary>
	/// <param name="soundDataIndex"></param>
	void SoundPlayWave(uint32_t soundDataIndex);

	/// <summary>
	/// ループ再生
	/// </summary>
	/// <param name="soundDataIndex"></param>
	void SoundPlayLoop(uint32_t soundDataIndex);

	/// <summary>
	/// 停止
	/// </summary>
	/// <param name="soundDataIndex"></param>
	void SoundPlayStop(uint32_t soundDataIndex);

private:
	Audio() = default;
	~Audio() = default;
	Audio(const Audio&) = default;
	const Audio& operator=(const Audio&) = delete;

private: // メンバ変数
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_ = nullptr;
	IXAudio2MasteringVoice* masterVoice_ = nullptr;
	SoundData soundData_[kMaxSoundData] = {};
	uint32_t soundDataIndex_ = 0;
	IXAudio2SourceVoice* pSourcevoice_[kMaxSoundData] = {};
};
