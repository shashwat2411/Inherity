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

D3DXVECTOR2 size = D3DXVECTOR2(384.0f, 177.0f);

void Animator::EngineDisplay()
{
	if (ImGui::TreeNode("Animator"))
	{
		std::string status = "Status : " + animation[animIndex]->status;
		ImGui::Text(status.c_str());
		ImGui::DragFloat2("size", size);

		ImGui::Begin("Animator", nullptr, ImGuiWindowFlags_NoMove);
		{
			if (ImPlot::BeginPlot(animation[animIndex]->name.c_str(), "Frame", "Value", ImVec2(size.x, size.y)))
			{
				std::vector<float> x;
				for (int i = 0; i < animation[animIndex]->data.size(); i++)
				{
					x.push_back((float)animation[animIndex]->data[i].frame);
				}

				std::vector<std::vector<float>> y;
				for (int i = 0; i < animation[animIndex]->keyframes; i++)
				{
					std::vector<float> temp;
					for (int j = 0; j < animation[animIndex]->data.size(); j++)
					{
						temp.push_back(animation[animIndex]->data[j].angle[i].move);
					}
					y.push_back(temp);
				}

				for (int i = 0; i < animation[animIndex]->keyName.size(); i++)
				{
					ImPlot::PlotLine(animation[animIndex]->keyName[i].c_str(), x.data(), y[i].data(), x.size());
				}


				{
					ImPlotAxisFlags ax_flags = ImPlotDragToolFlags_None;
					static ImPlotDragToolFlags flags = ImPlotDragToolFlags_None;

					//for (int i = 0; i < animation[animIndex]->keyName.size(); i++)
					{
						static AnimatorPoint cNode[4];
						static AnimatorPoint aNode[3] = { AnimatorPoint(x[0], y[0][0]), AnimatorPoint(x[1], y[0][1]), AnimatorPoint(x[2], y[0][2]) };

						aNode[0].plusDirection = D3DXVECTOR2((x[1] - x[0]) * 0.1f, (y[0][1] - y[0][0]) * 0.1f);

						aNode[1].minusDirection = D3DXVECTOR2((x[0] - x[1]) * 0.1f, (y[0][0] - y[0][1]) * 0.1f);
						aNode[1].plusDirection	= D3DXVECTOR2((x[2] - x[1]) * 0.1f, (y[0][2] - y[0][1]) * 0.1f);

						aNode[2].minusDirection = D3DXVECTOR2((x[1] - x[2]) * 0.1f, (y[0][1] - y[0][2]) * 0.1f);


						aNode[0].point = ImPlotPoint(x[0], y[0][0]);
						aNode[1].point = ImPlotPoint(x[1], y[0][1]);
						aNode[2].point = ImPlotPoint(x[2], y[0][2]);

						cNode[0].point = ImPlotPoint(x[0] + aNode[0].plusDirection.x, y[0][0] + aNode[0].plusDirection.y);

						cNode[1].point = ImPlotPoint(x[1] + aNode[1].minusDirection.x, y[0][1] + aNode[1].minusDirection.y);
						cNode[2].point = ImPlotPoint(x[1] + aNode[1].plusDirection.x, y[0][1] + aNode[1].plusDirection.y);

						cNode[3].point = ImPlotPoint(x[2] + aNode[2].minusDirection.x, y[0][2] + aNode[2].minusDirection.y);

						ImVec4 aNodeColor = ImVec4(0, 0.9f, 0, 1);
						ImVec4 cNodeColor = ImVec4(0, 0.5f, 1, 1);

						ImPlot::DragPoint(0, &aNode[0].point.x, &aNode[0].point.y, aNodeColor, 4, flags, &aNode[0].clicked, &aNode[0].hovered, &aNode[0].held);
						ImPlot::DragPoint(1, &cNode[0].point.x, &cNode[0].point.y, cNodeColor, 4, flags, &cNode[0].clicked, &cNode[0].hovered, &cNode[0].held);

						ImPlot::DragPoint(2, &cNode[1].point.x, &cNode[1].point.y, cNodeColor, 4, flags, &cNode[1].clicked, &cNode[1].hovered, &cNode[1].held);
						ImPlot::DragPoint(3, &aNode[1].point.x, &aNode[1].point.y, aNodeColor, 4, flags, &aNode[1].clicked, &aNode[1].hovered, &aNode[1].held);
						ImPlot::DragPoint(4, &cNode[2].point.x, &cNode[2].point.y, cNodeColor, 4, flags, &cNode[2].clicked, &cNode[2].hovered, &cNode[2].held);

						ImPlot::DragPoint(5, &cNode[3].point.x, &cNode[3].point.y, cNodeColor, 4, flags, &cNode[3].clicked, &cNode[3].hovered, &cNode[3].held);
						ImPlot::DragPoint(6, &aNode[2].point.x, &aNode[2].point.y, aNodeColor, 4, flags, &aNode[2].clicked, &aNode[2].hovered, &aNode[2].held);

						static ImPlotPoint B1[100];
						static ImPlotPoint B2[100];
						for (int i = 0; i < 100; ++i) {
							double t = i / 99.0;
							double u = 1 - t;
							double w1 = u * u*u;
							double w2 = 3 * u*u*t;
							double w3 = 3 * u*t*t;
							double w4 = t * t*t;
							B1[i] = ImPlotPoint(
								w1 * aNode[0].point.x +
									w2 * cNode[0].point.x +
									w3 * cNode[1].point.x +
									w4 * aNode[1].point.x,
								w1 * aNode[0].point.y +
									w2 * cNode[0].point.y + 
									w3 * cNode[1].point.y + 
									w4 * aNode[1].point.y
							);
							B2[i] = ImPlotPoint(
								w1 * aNode[1].point.x +
									w2 * cNode[2].point.x + 
									w3 * cNode[3].point.x + 
									w4 * aNode[2].point.x, 
								w1 * aNode[1].point.y +
									w2 * cNode[2].point.y + 
									w3 * cNode[3].point.y + 
									w4 * aNode[2].point.y
							);
						}

						int i = 1;
						std::string str = "##h" + std::to_string(i);

						static ImPlotPoint P1[2];
						static ImPlotPoint P2[2];
						static ImPlotPoint P3[2];
						static ImPlotPoint P4[2];

						P1[0] = aNode[0].point;
						P1[1] = cNode[0].point;

						P2[0] = aNode[1].point;
						P2[1] = cNode[1].point;

						P3[0] = aNode[1].point;
						P3[1] = cNode[2].point;

						P4[0] = aNode[2].point;
						P4[1] = cNode[3].point;

						ImVec4 pink = ImVec4(1, 0.5f, 1, 1);
						ImVec4 blue = ImVec4(0, 0.5f, 1, 1);
						ImVec4 red = ImVec4(1, 0, 0, 1);

						ImPlot::SetNextLineStyle(pink, cNode[0].hovered || cNode[0].held ? 2.0f : 1.0f);
						ImPlot::PlotLine(str.c_str(), &P1[0].x, &P1[0].y, 2, 0, 0, sizeof(ImPlotPoint));

						ImPlot::SetNextLineStyle(blue, cNode[1].hovered || cNode[1].held ? 2.0f : 1.0f);
						ImPlot::PlotLine("##h2", &P2[0].x, &P2[0].y, 2, 0, 0, sizeof(ImPlotPoint));

						ImPlot::SetNextLineStyle(blue, cNode[2].hovered || cNode[2].held ? 2.0f : 1.0f);
						ImPlot::PlotLine("##h3", &P3[0].x, &P3[0].y, 2, 0, 0, sizeof(ImPlotPoint));

						ImPlot::SetNextLineStyle(red, cNode[3].hovered || cNode[3].held ? 2.0f : 1.0f);
						ImPlot::PlotLine("##h4", &P4[0].x, &P4[0].y, 2, 0, 0, sizeof(ImPlotPoint));


						ImPlot::SetNextLineStyle(ImVec4(0, 0.9f, 0, 1), aNode[0].hovered || aNode[0].held || aNode[1].hovered || aNode[1].held ? 3.0f : 2.0f);
						ImPlot::PlotLine(animation[animIndex]->keyName[0].c_str(), &B1[0].x, &B1[0].y, 100, 0, 0, sizeof(ImPlotPoint));
						ImPlot::SetNextLineStyle(ImVec4(0, 0.9f, 0, 1), aNode[1].hovered || aNode[1].held || aNode[2].hovered || aNode[2].held ? 3.0f : 2.0f);
						ImPlot::PlotLine(animation[animIndex]->keyName[0].c_str(), &B2[0].x, &B2[0].y, 100, 0, 0, sizeof(ImPlotPoint));

						/*
						{
							bool clicked1[2] = { false };
							bool hovered1[2] = { false };
							bool held1[2] = { false };

							bool clicked2[3] = { false };
							bool hovered2[3] = { false };
							bool held2[3] = { false };

							bool clicked3[2] = { false };
							bool hovered3[2] = { false };
							bool held3[2] = { false };

							static ImPlotPoint P12 = ImPlotPoint((x[1] - x[0]) * 0.1f, (y[0][1] - y[0][0]) * 0.1f);
							static ImPlotPoint P21 = ImPlotPoint((x[0] - x[1]) * 0.1f, (y[0][0] - y[0][1]) * 0.1f);
							static ImPlotPoint P23 = ImPlotPoint((x[2] - x[1]) * 0.1f, (y[0][2] - y[0][1]) * 0.1f);
							static ImPlotPoint P32 = ImPlotPoint((x[1] - x[2]) * 0.1f, (y[0][1] - y[0][2]) * 0.1f);

							static ImPlotPoint P1[] = { ImPlotPoint(x[0],y[0][0]), ImPlotPoint(x[0] + P12.x, y[0][0] + P12.y) };
							static ImPlotPoint P2[] = { ImPlotPoint(x[1] + P21.x,y[0][1] + P21.y),  ImPlotPoint(x[1],y[0][1]), ImPlotPoint(x[1] + P23.x,y[0][1] + P23.y) };
							static ImPlotPoint P3[] = { ImPlotPoint(x[2] + P32.x,y[0][2] + P32.y),  ImPlotPoint(x[2],y[0][2]) };

							ImPlot::DragPoint(0, &P1[0].x, &P1[0].y, ImVec4(0, 0.9f, 0, 1), 4, flags, &clicked1[0], &hovered1[0], &held1[0]);
							ImPlot::DragPoint(1, &P1[1].x, &P1[1].y, ImVec4(1, 0.5f, 1, 1), 4, flags, &clicked1[1], &hovered1[1], &held1[1]);

							ImPlot::DragPoint(2, &P2[0].x, &P2[0].y, ImVec4(0, 0.5f, 1, 1), 4, flags, &clicked2[0], &hovered2[0], &held2[0]);
							ImPlot::DragPoint(3, &P2[1].x, &P2[1].y, ImVec4(0, 0.9f, 0, 1), 4, flags, &clicked2[1], &hovered2[1], &held2[1]);
							ImPlot::DragPoint(4, &P2[2].x, &P2[2].y, ImVec4(0, 0.5f, 1, 1), 4, flags, &clicked2[2], &hovered2[2], &held2[2]);

							ImPlot::DragPoint(5, &P3[0].x, &P3[0].y, ImVec4(0, 0.5f, 1, 1), 4, flags, &clicked3[0], &hovered3[0], &held3[0]);
							ImPlot::DragPoint(6, &P3[1].x, &P3[1].y, ImVec4(0, 0.9f, 0, 1), 4, flags, &clicked3[1], &hovered3[1], &held3[1]);

							static ImPlotPoint B1[100];
							static ImPlotPoint B2[100];
							for (int i = 0; i < 100; ++i) {
								double t = i / 99.0;
								double u = 1 - t;
								double w1 = u * u*u;
								double w2 = 3 * u*u*t;
								double w3 = 3 * u*t*t;
								double w4 = t * t*t;
								B1[i] = ImPlotPoint(w1*P1[0].x + w2 * P1[1].x + w3 * P2[0].x + w4 * P2[1].x, w1*P1[0].y + w2 * P1[1].y + w3 * P2[0].y + w4 * P2[1].y);
								B2[i] = ImPlotPoint(w1*P2[1].x + w2 * P2[2].x + w3 * P3[0].x + w4 * P3[1].x, w1*P2[1].y + w2 * P2[2].y + w3 * P3[0].y + w4 * P3[1].y);
							}

							int i = 1;
							std::string str = "##h" + std::to_string(i);

							ImPlot::SetNextLineStyle(ImVec4(1, 0.5f, 1, 1), hovered1[1] || held1[1] ? 2.0f : 1.0f);
							ImPlot::PlotLine(str.c_str(), &P1[0].x, &P1[0].y, 2, 0, 0, sizeof(ImPlotPoint));

							ImPlot::SetNextLineStyle(ImVec4(0, 0.5f, 1, 1), hovered2[0] || held2[0] ? 2.0f : 1.0f);
							ImPlot::PlotLine("##h2", &P2[0].x, &P2[0].y, 2, 0, 0, sizeof(ImPlotPoint));

							ImPlot::SetNextLineStyle(ImVec4(0, 0.5f, 1, 1), hovered2[2] || held2[2] ? 2.0f : 1.0f);
							ImPlot::PlotLine("##h3", &P2[1].x, &P2[1].y, 2, 0, 0, sizeof(ImPlotPoint));

							ImPlot::SetNextLineStyle(ImVec4(1, 0, 0, 1), hovered3[0] || held3[0] ? 2.0f : 1.0f);
							ImPlot::PlotLine("##h4", &P3[0].x, &P3[0].y, 2, 0, 0, sizeof(ImPlotPoint));


							ImPlot::SetNextLineStyle(ImVec4(0, 0.9f, 0, 1), hovered1[0] || held1[0] || hovered2[1] || held2[1] ? 3.0f : 2.0f);
							ImPlot::PlotLine(animation[animIndex]->keyName[0].c_str(), &B1[0].x, &B1[0].y, 100, 0, 0, sizeof(ImPlotPoint));
							ImPlot::SetNextLineStyle(ImVec4(0, 0.9f, 0, 1), hovered2[1] || held2[1] || hovered3[1] || held3[1] ? 3.0f : 2.0f);
							ImPlot::PlotLine(animation[animIndex]->keyName[0].c_str(), &B2[0].x, &B2[0].y, 100, 0, 0, sizeof(ImPlotPoint));
						}
						*/
					}
				}

				ImPlot::EndPlot();
			}

			ImGui::End();
		}

		ImGui::TreePop();
		ImGui::Spacing();
	}
}