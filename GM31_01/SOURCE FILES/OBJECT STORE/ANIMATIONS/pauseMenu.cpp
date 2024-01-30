#include "animations.h"

void PauseMenu::Start()
{
	name = "PauseMenu";

	//¶˜r
	data.push_back(AnimationData(0, Data(0.00f, &gameObject->transform->Scale.x)));
	data[0].angle.push_back(		Data(0.05f, &gameObject->transform->Scale.y));

	data.push_back(AnimationData(30, Data(1.0f, &gameObject->transform->Scale.x)));
	data[1].angle.push_back(		 Data(0.05f, &gameObject->transform->Scale.y));

	data.push_back(AnimationData(60, Data(1.0f, &gameObject->transform->Scale.x)));
	data[2].angle.push_back(		 Data(1.0f, &gameObject->transform->Scale.y));


	keyName.push_back("X");
	keyName.push_back("Y");

	color.push_back(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	color.push_back(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	timer = 0.0f;
	animationSize = 60;
	keyframes = 3;
	status = Animation::STANDBY;
}