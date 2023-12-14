#pragma once
#include "audio.h"
//#include <array>

#define DEFAULT_VOLUME 1.0f

class SoundReader
{
public:

	enum READ_SOUND
	{
		//SE
		GUARD,

		//BGM
		GAME,

		READ_SOUND_MAX
	};

private:
	static std::array<Audio, READ_SOUND_MAX>Audios;
	//static Audio Audios[SoundReader::READ_SOUND_MAX];

	static std::array<const char*, READ_SOUND_MAX> soundNames;
	//static const char* soundNames[SoundReader::READ_SOUND_MAX];

public:

	static void ReadSound()
	{/*
		Audio::InitMaster();*/

		//SE
		Audios[GUARD].Load					("asset\\sound\\wan.wav");

		//BGM
		Audios[GAME].Load					("asset\\sound\\bgm.wav");

		//NAMING
		soundNames[GUARD] = "GUARD";
		soundNames[GAME]  = "GAME";

		//for (int i = 0; i < SoundReader::READ_SOUND_MAX; i++)
		//{
		//	soundNames[i] = Audios[i].name.c_str();
		//}
	}

	static void UnloadAudio()
	{
		for (int i = 0; i < READ_SOUND_MAX; i++)
		{
			Audios[i].Unload();
		}

		Audio::UninitMaster();
	}

	static void StopAllSound()
	{
		for (int i = 0; i < READ_SOUND_MAX; i++)
		{
			Audios[i].Stop();
		}
	}

	static Audio* GetReadSound(READ_SOUND value)
	{
		return &Audios[value];
	}
	//static const char* const* GetSoundNames()
	//{
	//	return soundNames;
	//}
	static const char* const* GetSoundNames()
	{
		const char** names;
		names = &soundNames[0];
		return names;
	}

};