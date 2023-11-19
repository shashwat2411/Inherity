#include "component.h"
#include "soundReader.h"
#include "baseobjects.h"
#include "manager.h"

void AudioSource::Start()
{
	loop = false;
	parentActive = true;
	playOnAwake = true;
	threeDimension = false;

	volume = DEFAULT_VOLUME;
	volumePercentage = 100.0f;

	parentActive = gameObject->GetActive();
	clip = SoundReader::GetReadSound(SoundReader::GUARD);

	listeners = Manager::GetScene()->FindGameObjectsOfType<AudioListener>();
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


	if (threeDimension == true)
	{
		std::vector<float> dist;
		AudioListener* object = nullptr;
		GAMEOBJECT* obj = gameObject;

		for (auto obj : listeners)
		{
			if (obj != nullptr)
			{
				dist.push_back(gameObject->transform->DistanceFrom(obj->gameObject));
			}
		}

		if (listeners.size() > 1)
		{
			std::sort(listeners.begin(), listeners.end(), [obj](AudioListener* a, AudioListener* b) {
				return obj->transform->DistanceFrom(a->gameObject) < obj->transform->DistanceFrom(b->gameObject);
			});
		}

		object = listeners[0];

		float start = object->GetStartArea();
		float end = object->GetEndArea();

		float distance = gameObject->transform->DistanceFrom(object->gameObject);

		volumePercentage = (((end - distance) / (end - start)));
		if (distance <= start) { volumePercentage = 1.0f; }
		else if (distance > end) { volumePercentage = 0.0f; }

		clip->GetSourceVoice()->SetVolume(volumePercentage * volume);
	}
	else
	{
		clip->GetSourceVoice()->SetVolume(volume);
	}
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