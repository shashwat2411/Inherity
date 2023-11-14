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

		float frame1 = animation[animIndex]->data[animation[animIndex]->index].frame;
		float frame2 = animation[animIndex]->data[animation[animIndex]->index + 1].frame;

		dt = (float)(timer - frame1) / (float)(frame2 - frame1);

		for (int i = 0; i < animation[animIndex]->data[animation[animIndex]->index].angle.size(); i++)
		{
			float angle1 = animation[animIndex]->data[animation[animIndex]->index].angle[i].move;
			float angle2 = animation[animIndex]->data[animation[animIndex]->index + 1].angle[i].move;

			dangle = (angle2 - angle1);

			*animation[animIndex]->data[animation[animIndex]->index].angle[i].pointer = angle1 + (dt * dangle);
		}

		animation[animIndex]->timer++;

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