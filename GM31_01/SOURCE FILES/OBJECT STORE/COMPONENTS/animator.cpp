#include "component.h"

void Animator::Start()
{
	multiple = true;
}

void Animator::End()
{
	animation.clear();
}

void Animator::Update()
{
	if (animation[animIndex]->status == Animation::PLAYBACK || animation[animIndex]->status == Animation::LOOP)
	{
		float dt, dangle;
		float timer = animation[animIndex]->timer;

		float frame1 = (float)animation[animIndex]->data[animation[animIndex]->index].frame;
		float frame2 = (float)animation[animIndex]->data[animation[animIndex]->index + 1].frame;

		dt = (timer - frame1) / (frame2 - frame1);

		for (int i = 0; i < animation[animIndex]->data[animation[animIndex]->index].angle.size(); i++)
		{
			float angle1 = animation[animIndex]->data[animation[animIndex]->index].angle[i].move;
			float angle2 = animation[animIndex]->data[animation[animIndex]->index + 1].angle[i].move;

			dangle = (angle2 - angle1);

			*animation[animIndex]->data[animation[animIndex]->index].angle[i].pointer = angle1 + (dt * dangle);
		}

		animation[animIndex]->timer += Time::fixedTimeScale;

		if (animation[animIndex]->timer > frame2)
		{
			animation[animIndex]->index++;

			if (animation[animIndex]->index >= animation[animIndex]->data.size() - 1)
			{
				if (animation[animIndex]->status != Animation::LOOP) { InitAnimation(Animation::END); }
				else { InitAnimation(Animation::LOOP); }
			}
		}
	}
}

void Animator::Draw()
{

}

const char* status[Animation::ANIMATION_STATUS_MAX] =
{
	"STANDBY",
	"PLAYBACK",
	"END",
	"LOOP",
};

void Animator::EngineDisplay()
{
	if (ImGui::TreeNode("Animator"))
	{
		std::string status = "Status : " + status[animation[animIndex]->status];
		ImGui::Text(status.c_str());

		ImGui::TreePop();
		ImGui::Spacing();
	}
}