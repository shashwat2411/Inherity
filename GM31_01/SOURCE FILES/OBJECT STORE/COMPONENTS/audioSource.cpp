#include "component.h"
#include "soundReader.h"
#include "baseobjects.h"
#include "manager.h"

void AudioSource::Start()
{
	loop = false;
	parentActive = true;
	playOnAwake = true;

	volume = DEFAULT_VOLUME;

	parentActive = gameObject->GetActive();
	clip = SoundReader::GetReadSound(SoundReader::GUARD);
}

void AudioSource::End()
{

}

void AudioSource::Update()
{
	if (parentActive != gameObject->GetActive())
	{
		parentActive = gameObject->GetActive();
		clip->Play(loop, volume);
	}

	clip->GetSourceVoice()->SetVolume(volume);
}

void AudioSource::Draw()
{

}

void AudioSource::Play(bool l, float v)
{
	loop = l;
	volume = v;

	clip->Play(loop, volume);
}

void AudioSource::PlayOneShot(Audio* audio, float v)
{
	audio->Play(false, v);
}


SOUND* AudioSource::PlayClipAtPoint(Audio* clip, D3DXVECTOR3 position, float volume)
{
	SOUND* sound = Manager::GetScene()->AddGameObject<SOUND>();
	AudioSource* audio = sound->source;

	sound->transform->Position = position;
	audio->clip = clip;
	audio->PlayOneShot(audio->clip, volume);

	return sound;
}