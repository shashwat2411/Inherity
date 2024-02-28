#include "baseobjects.h"
#include "audio.h"

void SOUND::Init()
{
	Initialize();

	source = AddComponent<AudioSource>();
	source->SetThreeDimension(true);

	//ignorePause = true;
}

void SOUND::Update()
{
	if (source != nullptr)
	{
		if (source->GetClip()->IsPlaying() == false)
		{
			Destroy(true);
		}
	}
}