#include "animations.h"

void TitleDomeAnimation::Start()
{
	name = "TitleDomeAnimation";

	//¶˜r
	data.push_back(AnimationData(0, Data(0.0f, &gameObject->transform->Position.y)));

	data.push_back(AnimationData(60, Data(-1.0f, &gameObject->transform->Position.y)));

	data.push_back(AnimationData(120, Data(-1.0f, &gameObject->transform->Position.y)));

	data.push_back(AnimationData(240, Data(1.0f, &gameObject->transform->Position.y)));

	data.push_back(AnimationData(360, Data(1.0f, &gameObject->transform->Position.y)));


	keyName.push_back("Y");

	color.push_back(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	timer = 0.0f;
	animationSize = 360;
	keyframes = 5;
	status = Animation::STANDBY;
}