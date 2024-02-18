#pragma once
#include "audio.h"

#define DEFAULT_VOLUME 1.0f

class SoundReader
{
public:

	enum READ_SOUND
	{
		//SE
		GUARD,
		OPTION_SELECT,
		OPTION_CHANGE,
		KNIFE_DAMAGE,
		GAMEOVER,
		PLAYER_DEATH,
		SHOOT,
		SWING,
		ENEMY_DEATH,

		PAUSE_IN,
		PAUSE_OUT,

		BLOOD_IN,
		BLOOD_OUT,

		//BGM
		TITLE,
		BATTLE,
		ENEMY_DANCE,

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
		Audios[OPTION_SELECT].Load			("asset\\sound\\optionSelect.wav");
		Audios[OPTION_CHANGE].Load			("asset\\sound\\optionChange.wav");
		Audios[KNIFE_DAMAGE].Load			("asset\\sound\\Tomatos_Melancholy_2\\Knife.wav");
		Audios[GAMEOVER].Load				("asset\\sound\\Tomatos_Melancholy_2\\GameOver.wav");
		Audios[PLAYER_DEATH].Load			("asset\\sound\\Tomatos_Melancholy_2\\PlayerDeath.wav");
		Audios[SHOOT].Load					("asset\\sound\\Tomatos_Melancholy_2\\shoot.wav");
		Audios[SWING].Load					("asset\\sound\\Tomatos_Melancholy_2\\swing3.wav");
		Audios[ENEMY_DEATH].Load			("asset\\sound\\Tomatos_Melancholy_2\\EnemyDeath.wav");

		Audios[PAUSE_IN].Load				("asset\\sound\\Tomatos_Melancholy_2\\pauseIn.wav");
		Audios[PAUSE_OUT].Load				("asset\\sound\\Tomatos_Melancholy_2\\pauseOut.wav");

		Audios[BLOOD_IN].Load				("asset\\sound\\Tomatos_Melancholy_2\\bloodIn.wav");
		Audios[BLOOD_OUT].Load				("asset\\sound\\Tomatos_Melancholy_2\\bloodOut.wav");

		//BGM
		Audios[TITLE].Load					("asset\\sound\\Tomatos_Melancholy_2\\MainMenu.wav");
		Audios[BATTLE].Load					("asset\\sound\\Tomatos_Melancholy_2\\Battle.wav");
		Audios[ENEMY_DANCE].Load			("asset\\sound\\Tomatos_Melancholy_2\\enemyDancing.wav");

		//NAMING
		soundNames[GUARD]			= "GUARD";
		soundNames[OPTION_SELECT]	= "OPTION SELECT";
		soundNames[OPTION_CHANGE]	= "OPTION CHANGE";
		soundNames[KNIFE_DAMAGE]	= "KNIFE DAMAGE";
		soundNames[GAMEOVER]		= "GAME OVER";
		soundNames[PLAYER_DEATH]	= "PLAYER DEATH";
		soundNames[SHOOT]			= "SHOOT";
		soundNames[SWING]			= "SWING";
		soundNames[ENEMY_DEATH]		= "ENEMY DEATH";

		soundNames[PAUSE_IN]		= "PAUSE IN";
		soundNames[PAUSE_OUT]		= "PAUSE OUT";

		soundNames[BLOOD_IN]		= "BLOOD IN";
		soundNames[BLOOD_OUT]		= "BLOOD OUT";

		soundNames[TITLE]			= "TITLE";
		soundNames[BATTLE]			= "BATTLE";
		soundNames[ENEMY_DANCE]		= "ENEMY DANCE";

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