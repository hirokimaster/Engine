#include "Audio.h"

Audio* Audio::GetInstance()
{
	static Audio instanc;
	return &instanc;
}

void Audio::Initialize()
{
	HRESULT result{};

	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	result = xAudio2_->CreateMasteringVoice(&masterVoice_);

}

void Audio::Finalize()
{
	xAudio2_.Reset();
	SoundUnload();
}

uint32_t Audio::SoundLoadWave(const char* filename)
{
	++soundDataIndex_;
	std::ifstream file;
	file.open(filename, std::ios_base::binary);
	assert(file.is_open());

	RiffHeader riff = {};
	file.read((char*)&riff, sizeof(riff));
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	FormatChunk format = {};
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	ChunkHeader data = {};
	file.read((char*)&data, sizeof(data));
	if (strncmp(data.id, "JUNK", 4) == 0) {

		file.seekg(data.size, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0)
	{
		assert(0);
	}

	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	file.close();

	soundData_[soundDataIndex_].wfex = format.fmt;
	soundData_[soundDataIndex_].pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData_[soundDataIndex_].bufferSize = data.size;

	return soundDataIndex_;

}

void Audio::SoundUnload()
{
	for (int i = soundDataIndex_; soundDataIndex_ > 0; --i) {
		delete[] soundData_[i].pBuffer;
		soundData_[i].pBuffer = 0;
		soundData_[i].bufferSize = 0;
		soundData_[i].wfex = {};
	}

	soundDataIndex_ = 0;
}

void Audio::SoundPlayWave(uint32_t soundDataIndex)
{
	HRESULT result{};

	result = xAudio2_->CreateSourceVoice(&pSourcevoice_[soundDataIndex], &soundData_[soundDataIndex].wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData_[soundDataIndex].pBuffer;
	buf.AudioBytes = soundData_[soundDataIndex].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	result = pSourcevoice_[soundDataIndex]->SubmitSourceBuffer(&buf);
	result = pSourcevoice_[soundDataIndex]->Start();

	assert(SUCCEEDED(result));
}

void Audio::SoundPlayLoop(uint32_t soundDataIndex)
{
	HRESULT result{};

	result = xAudio2_->CreateSourceVoice(&pSourcevoice_[soundDataIndex], &soundData_[soundDataIndex].wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData_[soundDataIndex].pBuffer;
	buf.AudioBytes = soundData_[soundDataIndex].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	// ループできるように設定
	buf.LoopBegin = 0;
	buf.LoopLength = 0;
	buf.LoopCount = XAUDIO2_LOOP_INFINITE;

	result = pSourcevoice_[soundDataIndex]->SubmitSourceBuffer(&buf);
	result = pSourcevoice_[soundDataIndex]->Start();

	assert(SUCCEEDED(result));
}

void Audio::SoundPlayStop(uint32_t soundDataIndex)
{
	HRESULT result{};
	result = pSourcevoice_[soundDataIndex]->Stop();
	result = pSourcevoice_[soundDataIndex]->FlushSourceBuffers();

	assert(SUCCEEDED(result));
}