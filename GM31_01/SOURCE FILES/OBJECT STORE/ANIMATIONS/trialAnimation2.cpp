#include "animations.h"

void TrialAnimation2::Start()
{
	name = "TrialAnimation2";

	//¶˜r
	data.push_back(AnimationData(0, Data(0.0f, &gameObject->transform->Position.x)));
	data[0].angle.push_back(Data(2.0f, &gameObject->transform->Position.y));
	data[0].angle.push_back(Data(0.0f, &gameObject->transform->Position.z));

	data.push_back(AnimationData(60, Data(-1.0f, &gameObject->transform->Position.x)));
	data[1].angle.push_back(Data(2.0f, &gameObject->transform->Position.y));
	data[1].angle.push_back(Data(0.0f, &gameObject->transform->Position.z));

	data.push_back(AnimationData(120, Data(-1.0f, &gameObject->transform->Position.x)));
	data[2].angle.push_back(Data(3.0f, &gameObject->transform->Position.y));
	data[2].angle.push_back(Data(0.0f, &gameObject->transform->Position.z));

	data.push_back(AnimationData(240, Data(1.0f, &gameObject->transform->Position.x)));
	data[3].angle.push_back(Data(3.0f, &gameObject->transform->Position.y));
	data[3].angle.push_back(Data(0.0f, &gameObject->transform->Position.z));

	data.push_back(AnimationData(360, Data(1.0f, &gameObject->transform->Position.x)));
	data[4].angle.push_back(Data(3.0f, &gameObject->transform->Position.y));
	data[4].angle.push_back(Data(2.0f, &gameObject->transform->Position.z));


	keyName.push_back("X");
	keyName.push_back("Y");
	keyName.push_back("Z");

	color.push_back(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	color.push_back(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	color.push_back(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));

	timer = 0.0f;
	animationSize = 360;
	keyframes = 5;
	status = Animation::STANDBY;
}