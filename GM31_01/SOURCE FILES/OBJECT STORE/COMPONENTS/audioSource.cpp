#include "component.h"
#include "baseobjects.h"
#include "manager.h"

void AudioSource::Start()
{
	loop = false;
	parentActive = false;
	playOnAwake = false;
	threeDimension = false;

	soundIndex = 0;

	volume = DEFAULT_VOLUME;
	volumePercentage = 100.0f;

	clip = SoundReader::GetReadSound(SoundReader::GUARD);

	listeners = Manager::GetScene()->FindGameObjectsOfType<AudioListener>();
}

void AudioSource::End()
{

}

void AudioSource::Update()
{
	if (playOnAwake == true && parentActive != gameObject->GetActive())
	{
		parentActive = gameObject->GetActive();
		clip->Play(loop, volume);
	}
}

void AudioSource::Draw()
{
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

		if (listeners.empty() == false)
		{
			if (listeners[0] != nullptr)
			{
				object = listeners[0];

				float start = object->GetStartArea();
				float end = object->GetEndArea() + start;

				float distance = gameObject->transform->DistanceFrom(object->gameObject);

				volumePercentage = (((end - distance) / (end - start)));
				if (distance <= start) { volumePercentage = 1.0f; }
				else if (distance > end) { volumePercentage = 0.0f; }

				clip->GetSourceVoice()->SetVolume(volumePercentage * volume);
			}
		}
	}
	else
	{
		clip->GetSourceVoice()->SetVolume(volume);
	}
}

void AudioSource::EngineDisplay()
{
	if (ImGui::TreeNode("Audio Source"))
	{
		ImGui::Text("Clip : ");
		ImGui::SameLine();

		ImGui::PushID(0);
		if (ImGui::Combo("", &soundIndex, SoundReader::GetSoundNames(), SoundReader::READ_SOUND_MAX))
		{
			clip->Stop();
			SetClip((SoundReader::READ_SOUND)soundIndex);
		}
		ImGui::PopID();

		ImGui::Text("\n");
		if (ImGui::Button("Play")) { clip->Play(loop, volume); }
		ImGui::SameLine();
		if (ImGui::Button("Stop")) { clip->Stop(); }
		ImGui::Text("\n");

		DebugManager::BoolDisplay(&loop, -200.0f, "Loop", 2);
		ImGui::SameLine();
		DebugManager::BoolDisplay(&playOnAwake, -100.0f, "Play On Awake", 3);
		ImGui::SameLine();
		DebugManager::BoolDisplay(&threeDimension, -40.0f, "3D", 4);

		ImGui::Text("\n");

		DebugManager::FloatDisplay(&volume, -FLT_MIN, "Volume", false, D3DXVECTOR2(0.0f, 1.0f), 5);
		//ImGui::PushItemWidth(-FLT_MIN);
		//sprintf_s(str, sizeof(str), "Volume : %.2f", volume);
		//ImGui::SliderFloat(" ", &volume, 0.0f, 1.0f, str);

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

void AudioSource::SetClip(SoundReader::READ_SOUND index)
{
	clip = SoundReader::GetReadSound(index);

	soundIndex = index;
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

void AudioSource::CollectListeners(AudioListener* al)
{
	listeners.push_back(al);
}