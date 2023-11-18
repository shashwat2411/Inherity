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
		if (source->clip->IsPlaying() == false)
		{
			Destroy(true);
		}
	}
}