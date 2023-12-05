#include "animations.h"

void TrialAnimation::Start()
{
	name = "TrialAnimation";

	//¶˜r
	data.push_back(AnimationData(0, Data(0.0f, &gameObject->transform->Position.x)));
	data[0].angle.push_back(		Data(2.0f, &gameObject->transform->Position.y));

	data.push_back(AnimationData(60, Data(-1.0f, &gameObject->transform->Position.x)));
	data[1].angle.push_back(		 Data(2.0f, &gameObject->transform->Position.y));

	data.push_back(AnimationData(120, Data(-1.0f, &gameObject->transform->Position.x)));
	data[2].angle.push_back(		 Data(3.0f, &gameObject->transform->Position.y));


	keyName.push_back("X");
	keyName.push_back("Y");

	index = 0;
	timer = 0.0f;
	animationSize = 120;
	keyframes = 3;
	status = Animation::STANDBY;
}