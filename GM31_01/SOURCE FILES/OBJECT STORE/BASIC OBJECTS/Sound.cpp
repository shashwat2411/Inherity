#include "baseobjects.h"
#include "audio.h"

void SOUND::Init()
{
	Initialize();

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