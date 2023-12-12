#include "component.h"
#include "input.h"
#include "animations.h"
#include "manager.h"
#include <fstream>

#define INTERPOLATE

void Animator::Start()
{
	multiple = true;

	animIndex = 0;

	cNodeColor = ImVec4(0, 0.5f, 1, 1);
}

void Animator::End()
{
	animation.clear();
}

void Animator::Update()
{
	if (animation.size() > 0)
	{
		if (animation[animIndex]->status == Animation::PLAYBACK || animation[animIndex]->status == Animation::LOOP)
		{
#ifdef INTERPOLATE
			float dt, dangle;
			float frame1, frame2;
			std::vector<int> index1;
			std::vector<int> index2;

			float timer = animation[animIndex]->timer;

			index1 = animation[animIndex]->index;
			index2 = animation[animIndex]->index;
			for (int i = 0; i < index2.size(); i++)
			{
				index2[i]++;
			}


			for (int num = 0; num < animation[animIndex]->keyName.size(); num++)
			{
				int i = animation[animIndex]->bezierIndex[num];//index of the Bezier line

				frame1 = (float)B[animIndex][num][i][index1[num]].x;
				frame2 = (float)B[animIndex][num][i][index2[num]].x;

				dt = (timer - frame1) / (frame2 - frame1);

				float angle1 = (float)B[animIndex][num][i][index1[num]].y;
				float angle2 = (float)B[animIndex][num][i][index2[num]].y;

				dangle = (angle2 - angle1);

				*animation[animIndex]->data[i].angle[num].pointer = angle1 + (dt * dangle);
			}

			animation[animIndex]->timer += Time::fixedTimeScale;

			for (int num = 0; num < animation[animIndex]->keyName.size(); num++)
			{
				int i = animation[animIndex]->bezierIndex[num];//index of the Bezier line

				if (animation[animIndex]->timer > B[animIndex][num][i][index2[num]].x)
				{
					animation[animIndex]->index[num]++;

					if (animation[animIndex]->timer > B[animIndex][num][i].back().x)
					{
						animation[animIndex]->bezierIndex[num]++;
						animation[animIndex]->index[num] = 0;

						if (animation[animIndex]->bezierIndex[num] >= animation[animIndex]->keyframes - 1)
						{
							if (animation[animIndex]->status != Animation::LOOP) { InitAnimation(Animation::END); }
							else { InitAnimation(Animation::LOOP); }
						}
					}
				}
			}
#else INTERPOLATE
			float dt, dangle;
			std::vector<int> index;

			float timer = animation[animIndex]->timer;

			index = animation[animIndex]->index;


			for (int num = 0; num < animation[animIndex]->keyName.size(); num++)
			{
				int i = animation[animIndex]->bezierIndex[num];//index of the Bezier line

				float angle = (float)B[animIndex][num][i][index[num]].y;

				*animation[animIndex]->data[i].angle[num].pointer = angle;
			}

			animation[animIndex]->timer += Time::fixedTimeScale;

			for (int num = 0; num < animation[animIndex]->keyName.size(); num++)
			{
				int i = animation[animIndex]->bezierIndex[num];//index of the Bezier line

				if (animation[animIndex]->timer > B[animIndex][num][i][index[num]].x)
				{
					animation[animIndex]->index[num]++;

					if (animation[animIndex]->timer > B[animIndex][num][i].back().x)
					{
						animation[animIndex]->bezierIndex[num]++;
						animation[animIndex]->index[num] = 0;

						if (animation[animIndex]->bezierIndex[num] >= animation[animIndex]->keyframes - 1)
						{
							if (animation[animIndex]->status != Animation::LOOP) { InitAnimation(Animation::END); }
							else { InitAnimation(Animation::LOOP); }
						}
					}
				}
			}


#endif

		}
	}
}

void Animator::Draw()
{
	if (animation.size() > 0)
	{
		if (DebugManager::play == false)
		{
			if (Input::GetKeyTrigger('K'))
			{
				Save();
			}

			int frame = animation[animIndex]->data[0].frame;
			int i = (int)animation[animIndex]->data.size() - 1;
			for (int n = 0; n < animation[animIndex]->data.size(); n++)
			{
				if ((int)animation[animIndex]->timer < animation[animIndex]->data[n].frame)
				{
					i = n;
					if (i == 0)
					{
						int a = 0;
					}
					break;
				}
			}
			i = (i <= 0 ? 1 : i);

			for (int num = 0; num < animation[animIndex]->keyName.size(); num++)
			{
				int t = (int)animation[animIndex]->timer;
				int t1 = animation[animIndex]->data[i - 1].frame;
				int t2 = animation[animIndex]->data[i].frame;

				t = (t <= animation[animIndex]->data.back().frame ? t : animation[animIndex]->data.back().frame);
				t = (t >= 0 ? t : 0);

				int index = (t - t1) * (BEZIER_NUM - 1) / (t2 - t1);

				float angle = (float)B[animIndex][num][i - 1][index].y;

				*animation[animIndex]->data[i - 1].angle[num].pointer = angle;
			}
		}
	}
}

D3DXVECTOR2 size = D3DXVECTOR2(667.0f, 177.0f);//D3DXVECTOR2(384.0f, 177.0f);
const char* animationStatus[Animation::ANIMATION_STATUS_MAX] =
{
	"STANDBY",
	"PLAYBACK",
	"END",
	"LOOP"
};

void Animator::EngineDisplay()
{
	if (ImGui::TreeNode("Animator"))
	{
		if (ImGui::Button("Add Animation")) { ImGui::OpenPopup("Animation"); }
		if (ImGui::BeginPopup("Animation"))
		{
			ImGui::SeparatorText("Animation");
			if (ImGui::Selectable("CharacterRetract"))	{ AddAnimation<CharacterRetract>(); }
			if (ImGui::Selectable("TrialAnimation"))	{ AddAnimation<TrialAnimation>(); }
			if (ImGui::Selectable("TrialAnimation2"))	{ AddAnimation<TrialAnimation2>(); }
			ImGui::EndPopup();
		}

		if (animation.size() > 0)
		{
			int stat = (int)animation[animIndex]->status;
			ImGui::PushItemWidth(-FLT_MIN);
			ImGui::SliderInt(" ", &stat, 0, Animation::ANIMATION_STATUS_MAX - 1, animationStatus[stat]);
			//ImGui::DragFloat2("size", size);

			std::string str = "\n Time : " + std::to_string((int)animation[animIndex]->timer);
			ImGui::Text(str.c_str());

			ImGui::Begin("Animator", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
			{
				ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
				if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
				{
					for (int a = 0; a < animation.size(); a++)
					{
						if (ImGui::BeginTabItem(animation[a]->name.c_str()))
						{
							if (ImPlot::BeginPlot(animation[a]->name.c_str(), "Frame", "Value", ImVec2(size.x, size.y)))
							{
								{
									ImPlotAxisFlags ax_flags = ImPlotDragToolFlags_None;
									static ImPlotDragToolFlags flags = ImPlotDragToolFlags_None;

									{
										int id = 0;
										for (int num = 0; num < animation[a]->keyName.size(); num++)
										{
											aNodeColor = ImVec4(animation[a]->color[num]);

											int c = 0;
											for (int i = 0; i < animation[a]->keyframes; i++)
											{
												if (i == 0)
												{
													ImPlot::DragPoint(id, &aNode[a][num][i].point.x, &aNode[a][num][i].point.y, aNodeColor, 4, flags, &aNode[a][num][i].clicked, &aNode[a][num][i].hovered, &aNode[a][num][i].held);
													id++;

													ImPlot::DragPoint(id, &cNode[a][num][c].point.x, &cNode[a][num][c].point.y, cNodeColor, 4, flags, &cNode[a][num][c].clicked, &cNode[a][num][c].hovered, &cNode[a][num][c].held);
													id++;
													c++;
												}
												else if (i == animation[a]->keyframes - 1)
												{
													ImPlot::DragPoint(id++, &cNode[a][num][c].point.x, &cNode[a][num][c].point.y, cNodeColor, 4, flags, &cNode[a][num][c].clicked, &cNode[a][num][c].hovered, &cNode[a][num][c].held);
													id++;
													c++;

													ImPlot::DragPoint(id++, &aNode[a][num][i].point.x, &aNode[a][num][i].point.y, aNodeColor, 4, flags, &aNode[a][num][i].clicked, &aNode[a][num][i].hovered, &aNode[a][num][i].held);
													id++;
												}
												else
												{
													ImPlot::DragPoint(id, &cNode[a][num][c].point.x, &cNode[a][num][c].point.y, cNodeColor, 4, flags, &cNode[a][num][c].clicked, &cNode[a][num][c].hovered, &cNode[a][num][c].held);
													id++;
													c++;

													ImPlot::DragPoint(id, &aNode[a][num][i].point.x, &aNode[a][num][i].point.y, aNodeColor, 4, flags, &aNode[a][num][i].clicked, &aNode[a][num][i].hovered, &aNode[a][num][i].held);
													id++;

													ImPlot::DragPoint(id, &cNode[a][num][c].point.x, &cNode[a][num][c].point.y, cNodeColor, 4, flags, &cNode[a][num][c].clicked, &cNode[a][num][c].hovered, &cNode[a][num][c].held);
													id++;
													c++;
												}
											}

											for (int i = 0; i < animation[a]->keyframes - 1; i++)
											{
												for (int p = 0; p < BEZIER_NUM; ++p) {
													double t = p / double(BEZIER_NUM - 1);
													double u = 1 - t;
													double w1 = u * u*u;
													double w2 = 3 * u*u*t;
													double w3 = 3 * u*t*t;
													double w4 = t * t*t;
													B[a][num][i][p] = ImPlotPoint(
														w1 * aNode[a][num][i].point.x +
														w2 * cNode[a][num][i * 2].point.x +
														w3 * cNode[a][num][(i + 1) * 2 - 1].point.x +
														w4 * aNode[a][num][i + 1].point.x,
														w1 * aNode[a][num][i].point.y +
														w2 * cNode[a][num][i * 2].point.y +
														w3 * cNode[a][num][(i + 1) * 2 - 1].point.y +
														w4 * aNode[a][num][i + 1].point.y
													);
												}
											}

											for (int i = 0; i < c; i++)
											{
												std::string str = "##h" + std::to_string(i);

												ImPlotPoint P[2];

												P[0] = aNode[a][num][(i + 1) / 2].point;
												P[1] = cNode[a][num][i].point;

												ImVec4 color = cNodeColor;

												ImPlot::SetNextLineStyle(color, cNode[a][num][i].hovered || cNode[a][num][i].held ? 2.0f : 1.0f);
												ImPlot::PlotLine(str.c_str(), &P[0].x, &P[0].y, 2, 0, 0, sizeof(ImPlotPoint));
											}

											for (int i = 0; i < animation[a]->keyframes - 1; i++)
											{
												ImPlot::SetNextLineStyle(aNodeColor, aNode[a][num][i].hovered || aNode[a][num][i].held || aNode[a][num][i + 1].hovered || aNode[a][num][i + 1].held ? 3.0f : 2.0f);
												ImPlot::PlotLine(animation[a]->keyName[num].c_str(), &B[a][num][i][0].x, &B[a][num][i][0].y, BEZIER_NUM, 0, 0, sizeof(ImPlotPoint));
											}

										}

										//ImPlot::SetupAxesLimits(0, 1, 0, 1);
										double x = animation[animIndex]->timer;
										ImPlot::DragLineX(2, &x, ImVec4(1, 1, 1, 1), 1, flags);
										animation[animIndex]->timer = (float)x;
									}
								}

								ImPlot::EndPlot();

							}

							ImGui::EndTabItem();
						}
					}

					ImGui::EndTabBar();
				}

				ImGui::End();
			}
		}

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

void Animator::AnimatorPlotInit()
{
	std::vector<float> x;
	for (int i = 0; i < animation[animIndex]->keyframes; i++)
	{
		x.push_back((float)animation[animIndex]->data[i].frame);
	}

	std::vector<std::vector<float>> y;
	for (int i = 0; i < animation[animIndex]->keyName.size(); i++)
	{
		std::vector<float> temp;
		for (int j = 0; j < animation[animIndex]->keyframes; j++)
		{
			temp.push_back(animation[animIndex]->data[j].angle[i].move);
		}
		y.push_back(temp);
	}

	for (int num = 0; num < animation[animIndex]->keyName.size(); num++)
	{
		aNode[animIndex].emplace_back();
		cNode[animIndex].emplace_back();
		B[animIndex].emplace_back();

		for (int i = 0; i < animation[animIndex]->keyframes; i++)
		{
			aNode[animIndex][num].push_back(AnimatorPoint(x[i], y[num][i]));

			if (i == 0)
			{
				aNode[animIndex][num][i].plusDirection = D3DXVECTOR2((x[i + 1] - x[i]) * 0.1f, (y[num][i + 1] - y[num][i]) * 0.1f);

				cNode[animIndex][num].push_back(AnimatorPoint(x[i] + aNode[animIndex][num][i].plusDirection.x, y[num][i] + aNode[animIndex][num][i].plusDirection.y));
			}
			else if (i == animation[animIndex]->keyframes - 1)
			{
				aNode[animIndex][num][i].minusDirection = D3DXVECTOR2((x[i - 1] - x[i]) * 0.1f, (y[num][i - 1] - y[num][i]) * 0.1f);

				cNode[animIndex][num].push_back(AnimatorPoint(x[i] + aNode[animIndex][num][i].minusDirection.x, y[num][i] + aNode[animIndex][num][i].minusDirection.y));
			}
			else
			{
				aNode[animIndex][num][i].minusDirection = D3DXVECTOR2((x[i - 1] - x[i]) * 0.1f, (y[num][i - 1] - y[num][i]) * 0.1f);
				aNode[animIndex][num][i].plusDirection = D3DXVECTOR2((x[i + 1] - x[i]) * 0.1f, (y[num][i + 1] - y[num][i]) * 0.1f);

				cNode[animIndex][num].push_back(AnimatorPoint(x[i] + aNode[animIndex][num][i].minusDirection.x, y[num][i] + aNode[animIndex][num][i].minusDirection.y));
				cNode[animIndex][num].push_back(AnimatorPoint(x[i] + aNode[animIndex][num][i].plusDirection.x, y[num][i] + aNode[animIndex][num][i].plusDirection.y));
			}
		}

		for (int i = 0; i < animation[animIndex]->keyframes - 1; i++)
		{
			B[animIndex][num].emplace_back();
		}

		for (int i = 0; i < animation[animIndex]->keyframes - 1; i++)
		{
			for (int p = 0; p < BEZIER_NUM; ++p) {
				double t = p / double(BEZIER_NUM - 1);
				double u = 1 - t;
				double w1 = u * u*u;
				double w2 = 3 * u*u*t;
				double w3 = 3 * u*t*t;
				double w4 = t * t*t;
				B[animIndex][num][i][p] = ImPlotPoint(
					w1 * aNode[animIndex][num][i].point.x +
					w2 * cNode[animIndex][num][i * 2].point.x +
					w3 * cNode[animIndex][num][(i + 1) * 2 - 1].point.x +
					w4 * aNode[animIndex][num][i + 1].point.x,
					w1 * aNode[animIndex][num][i].point.y +
					w2 * cNode[animIndex][num][i * 2].point.y +
					w3 * cNode[animIndex][num][(i + 1) * 2 - 1].point.y +
					w4 * aNode[animIndex][num][i + 1].point.y
				);
			}
		}
	}

	Open();
	BezierCalculate();
}

void Animator::BezierCalculate()
{
	for (int a = 0; a < animation.size(); a++)
	{
		for (int num = 0; num < animation[animIndex]->keyName.size(); num++)
		{
			for (int i = 0; i < animation[a]->keyframes - 1; i++)
			{
				for (int p = 0; p < BEZIER_NUM; ++p) {
					double t = p / double(BEZIER_NUM - 1);
					double u = 1 - t;
					double w1 = u * u*u;
					double w2 = 3 * u*u*t;
					double w3 = 3 * u*t*t;
					double w4 = t * t*t;
					B[a][num][i][p] = ImPlotPoint(
						w1 * aNode[a][num][i].point.x +
						w2 * cNode[a][num][i * 2].point.x +
						w3 * cNode[a][num][(i + 1) * 2 - 1].point.x +
						w4 * aNode[a][num][i + 1].point.x,
						w1 * aNode[a][num][i].point.y +
						w2 * cNode[a][num][i * 2].point.y +
						w3 * cNode[a][num][(i + 1) * 2 - 1].point.y +
						w4 * aNode[a][num][i + 1].point.y
					);
				}
			}
		}
	}
}

void Animator::Save()
{
	for (int a = 0; a < animation.size(); a++)
	{
		std::string fileName = "asset\\animations\\" + animation[a]->name + ".txt";
		std::ofstream outFile(fileName.c_str());

		if (!outFile.is_open()) { return; }

		cereal::JSONOutputArchive archive(outFile);

		archive(CEREAL_NVP(aNode[a]), CEREAL_NVP(cNode[a]));
	}

}

void Animator::Open()
{
	for (int a = 0; a < animation.size(); a++)
	{
		std::string fileName = "asset\\animations\\" + animation[a]->name + ".txt";
		std::ifstream inFile(fileName.c_str());

		if (!inFile.is_open()) { return; }

		cereal::JSONInputArchive archive(inFile);

		archive(CEREAL_NVP(aNode[a]), CEREAL_NVP(cNode[a]));
	}
}

void Animator::AnimationAdder()
{
	Manager::GetScene()->animationAdder.push_back(AddComponentSaveFile(animation[animIndex]->name.c_str(), gameObject->GetTag().c_str()));
}