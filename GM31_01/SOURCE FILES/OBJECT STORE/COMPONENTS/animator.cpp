#include "component.h"
#include "../imGUI/implot.h"
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

		int index1 = animation[animIndex]->index;
		int index2 = animation[animIndex]->index + 1;

		float frame1 = (float)animation[animIndex]->data[index1].frame;
		float frame2 = (float)animation[animIndex]->data[index2].frame;

		dt = (timer - frame1) / (frame2 - frame1);

		for (int i = 0; i < animation[animIndex]->data[index1].angle.size(); i++)
		{
			float angle1 = animation[animIndex]->data[index1].angle[i].move;
			float angle2 = animation[animIndex]->data[index2].angle[i].move;

			dangle = (angle2 - angle1);

			*animation[animIndex]->data[index1].angle[i].pointer = angle1 + (dt * dangle);
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
		std::string status = "Status : " + animation[animIndex]->status;
		ImGui::Text(status.c_str());


		ImGui::Text("ImPlot Example");

		// Create a simple line plot
		if (ImPlot::BeginPlot("Line Plot", "X-axis", "Y-axis")) {
			// Sample data
			float xs[] = { 0.0, 1.0, 2.0, 3.0, 4.0 };
			float ys[] = { 0.0, 1.0, 0.5, 1.5, 1.0 };

			// Plot the data
			ImPlot::PlotLine("Line", xs, ys, IM_ARRAYSIZE(xs));

			// End the plot
			ImPlot::EndPlot();
		}

		//std::vector<const float*> floats;
		//int i = 0;
		//for (int i = 0; i < animation[animIndex]->data[animation[animIndex]->index].angle.size(); i++)
		//{
		//	floats.push_back(&animation[animIndex]->data[animation[animIndex]->index].angle[i].move);
		//}
		//const float** floater = floats.data();
		//ImGui::PlotLines("Frame Times", *floater, floats.size());

		ImGui::TreePop();
		ImGui::Spacing();
	}
}