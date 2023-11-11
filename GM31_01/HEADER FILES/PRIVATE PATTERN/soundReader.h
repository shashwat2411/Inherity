#pragma once
#include "../OBJECT STORE/component.h"

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
	static Audio Audios[SoundReader::READ_SOUND_MAX];

public:

	static void ReadSound()
	{
		//SE
		Audios[GUARD].Load					("asset\\sound\\wan.wav");

		//BGM
		Audios[GAME].Load					("asset\\sound\\bgm.wav");
	}

	static Audio* GetReadSound(READ_SOUND value)
	{
		return &Audios[value];
	}

	static void StopAllSound()
	{
		for (int i = 0; i < READ_SOUND_MAX; i++)
		{
			Audios[i].Stop();
		}
	}

	static void UnloadAudio()
	{
		for (int i = 0; i < READ_SOUND_MAX; i++)
		{
			Audios[i].Unload();
		}
	}

	static void PlayMaster(const char* value, float volume, float loop = false)
	{
		Audio* audio = Manager::GetScene()->AddGameObject<SOUND>(GAMEOBJECT_LAYER)->audio;
		audio->Load(value);
		audio->Play(loop, volume);
	}
};