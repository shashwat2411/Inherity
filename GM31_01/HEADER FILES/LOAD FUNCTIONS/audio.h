#pragma once
#include "main.h"
#include "renderer.h"
#include <xaudio2.h>

#define DEFAULT_VOLUME 1.0f

class Audio
{
private:
	BYTE* m_SoundData{};

	IXAudio2SourceVoice* m_SourceVoice{};

	static IXAudio2* m_Xaudio;
	static IXAudio2MasteringVoice* m_MasteringVoice;

public:
	int m_Length;
	int m_PlayLength;

public:

	static void InitMaster();
	static void UninitMaster();

	bool IsPlaying();
	void Load(const char *FileName);
	void Play(bool Loop = false, float volume = DEFAULT_VOLUME);
	void Unload();
	void SetVolume(float volume);
	void Stop();

	IXAudio2SourceVoice* GetSourceVoice() { return m_SourceVoice; }
};