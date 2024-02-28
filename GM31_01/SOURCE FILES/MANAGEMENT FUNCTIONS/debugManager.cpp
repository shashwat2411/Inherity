#include "debugManager.h"
#include "manager.h"
#include "input.h"
#include "customScenes.h"
#include "../saveFunctions.h"

bool pressedDebug[2] = { false };

bool DebugManager::play = true;
bool DebugManager::paused = false;
bool DebugManager::gizmo = false;
bool DebugManager::show_demo_window = false;
bool DebugManager::debugger = false;

bool show_plot_demo_window = false;
bool starter = false;
bool showCursor = true;
bool cursorOverride = false;

int DebugManager::index = 0;
int DebugManager::layer = 0;

float DebugManager::speed = 0.1f;

const char* str[MAX_LAYER] =
{
	"CAMERA_LAYER",
	"GAMEOBJECT_LAYER",
	"LATEOBJECT_LAYER",
	"COLLIDER_LAYER",
	"MAP_LAYER",
	"GIZMO_LAYER",
	"BILLBOARD_LAYER",
	"SHADOW_LAYER",
	"SPRITE_LAYER",
	"FADE_LAYER"
};

EDIT_MODE edit = EDIT_MODE::POSITION;

void DebugManager::Init()
{
#ifdef DEBUG

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(GetWindow());
	ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());


	io.Fonts->AddFontDefault();

	starter = true;
	play = false;
	paused = false;
	gizmo = true;

	Time::timeScale = 1.0f;
	Time::fixedTimeScale = 60.0f / FRAME_RATE;
	Time::deltaTime = 1.0f / FRAME_RATE;

#else
	debugger = false;
	starter = true;
#endif

	if (debugger == false || starter == true)
	{
		starter = true;
		play = true;
		paused = false;
		pressedDebug[0] = true;
		showCursor = false;
		ShowCursor(false);
	}
}

void DebugManager::Uninit()
{
#ifdef DEBUG
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
#endif
}

void DebugManager::Update()
{
#ifdef DEBUG

	// フレームの開始
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame(SCREEN_WIDTH, SCREEN_HEIGHT);//こいつを画面の解像度分にするように改造する
	ImGui::NewFrame();
#endif

	if (Input::GetKeyTrigger(VK_F10))
	{
		if (debugger == true) { debugger = false; }
		else if (debugger == false) { debugger = true; }
	}

	if (Input::GetKeyTrigger(VK_F11))
	{
		if (cursorOverride == true) { cursorOverride = false; ShowCursor(false); }
		else if (cursorOverride == false) { cursorOverride = true; ShowCursor(true); }
	}

}

void DebugManager::Draw()
{
#ifdef DEBUG
	ImGui::EndFrame();
	// ImGuiコード
	//フレームの描画
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (starter == false && LOAD_SCENE::GetLogo() == false)
	{ 
		play = false; 
		starter = true;
		Time::timeScale = 0.0f;
	}
#endif
}

float s = 30.0f;
void DebugManager::DebugDraw(SCENE * scene)
{
	if (debugger == true)
	{
#ifdef DEBUG
		//1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		if (show_plot_demo_window)
			ImPlot::ShowDemoWindow(&show_plot_demo_window);



		std::vector<GAMEOBJECT*> vector = scene->GetGameObjectListVector((LAYER)layer);

		//Index
		{
			ImGui::Begin("Index", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			ImGui::PushItemWidth(-FLT_MIN);
			ImGui::SliderInt(" ", &layer, 0, MAX_LAYER - 1, str[layer]);

			if (index > (int)vector.size() - 1) { index = (int)vector.size() - 1; }
			ImGui::PushItemWidth(-FLT_MIN);
			if (vector[0] != nullptr) { ImGui::SliderInt("", &index, 0, (int)vector.size() - 1, vector[index]->GetTag().c_str()); }

			if (Input::GetKeyPress(VK_SHIFT))
			{
				if (Input::GetKeyTrigger(VK_ADD)) { if (layer < MAX_LAYER - 1) { layer++; } else { layer = 0; } }
				if (Input::GetKeyTrigger(VK_SUBTRACT)) { if (layer > 0) { layer--; } else { layer = MAX_LAYER - 1; } }
			}
			else
			{
				if (Input::GetKeyTrigger(VK_ADD)) { if (index < (int)vector.size() - 1) { index++; } else { index = 0; } }
				if (Input::GetKeyTrigger(VK_SUBTRACT)) { if (index > 0) { index--; } else { index = (int)vector.size() - 1; } }
			}

			ImGui::End();
		}

		//Inspector
		{
			//ImGui::SetNextWindowSize(ImVec2(200, 170));
			ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
			{
				if (vector[0] != nullptr)
				{
					if (play == false || paused == true)
					{
						if (Input::GetKeyTrigger('1')) { edit = EDIT_MODE::POSITION; }
						if (Input::GetKeyTrigger('2')) { edit = EDIT_MODE::ROTATION; }
						if (Input::GetKeyTrigger('3')) { edit = EDIT_MODE::SCALE; }

						GAMEOBJECT* mover = vector[index];
						if (Input::GetKeyPress(VK_LSHIFT)) { mover = scene->GetCamera(); }

						D3DXVECTOR3 directionX(0.0f, 0.0f, 0.0f);
						D3DXVECTOR3 directionY(0.0f, 0.0f, 0.0f);
						D3DXVECTOR3 directionZ(0.0f, 0.0f, 0.0f);

						if (!Input::GetKeyPress(VK_CONTROL))
						{
							if (edit != EDIT_MODE::ROTATION)
							{
								if (Input::GetKeyPress('W')) { /*mover->transform->Position.z += 0.1f;*/ directionZ = scene->GetCamera()->camera->GetForward(); }
								else if (Input::GetKeyPress('S')) { /*mover->transform->Position.z -= 0.1f;*/ directionZ = -scene->GetCamera()->camera->GetForward(); }
								if (Input::GetKeyPress('Q')) { /*mover->transform->Position.y += 0.1f;*/ directionY = scene->GetCamera()->camera->GetUp(); }
								else if (Input::GetKeyPress('E')) { /*mover->transform->Position.y -= 0.1f;*/ directionY = -scene->GetCamera()->camera->GetUp(); }
								if (Input::GetKeyPress('D')) { /*mover->transform->Position.x += 0.1f;*/ directionX = scene->GetCamera()->camera->GetRight(); }
								else if (Input::GetKeyPress('A')) { /*mover->transform->Position.x -= 0.1f;*/ directionX = -scene->GetCamera()->camera->GetRight(); }
							}
							else
							{
								if (Input::GetKeyPress('W')) { /*mover->transform->Position.z += 0.1f;*/ directionZ = mover->transform->GetRightDirection(); }
								else if (Input::GetKeyPress('S')) { /*mover->transform->Position.z -= 0.1f;*/ directionZ = -mover->transform->GetRightDirection(); }
								if (Input::GetKeyPress('Q')) { /*mover->transform->Position.y += 0.1f;*/ directionY = mover->transform->GetUpDirection(); }
								else if (Input::GetKeyPress('E')) { /*mover->transform->Position.y -= 0.1f;*/ directionY = -mover->transform->GetUpDirection(); }
								if (Input::GetKeyPress('D')) { /*mover->transform->Position.x += 0.1f;*/ directionX = -mover->transform->GetForwardDirection(); }
								else if (Input::GetKeyPress('A')) { /*mover->transform->Position.x -= 0.1f;*/ directionX = mover->transform->GetForwardDirection(); }
							}
						}

						directionX.y = 0.0f;
						directionZ.y = 0.0f;

						directionY.x = 0.0f;
						directionY.z = 0.0f;

						D3DXVECTOR3 direction = directionX + directionY + directionZ;
						D3DXVec3Normalize(&direction, &direction);

						switch (edit)
						{
						case EDIT_MODE::POSITION:	speed = 0.1f; mover->transform->Position += direction * speed;	break;
						case EDIT_MODE::ROTATION:	speed = 0.8f; mover->transform->Rotation += direction * speed;	break;
						case EDIT_MODE::SCALE:		speed = 0.01f; mover->transform->Scale += direction * speed;	break;
						default: break;
						}
					}

					vector[index]->EngineDisplay();
#endif
#ifdef DEBUG

					int i = 0;
					for (auto component : vector[index]->GetComponentList())
					{
						ImGui::SeparatorText("");
						ImGui::SameLine();

						ImGui::PushID(i);
						ImGui::Checkbox("", component->GetEnabledPointer());
						ImGui::PopID();
						i++;

						ImGui::SameLine();
						ImGui::PushID(i);
						component->EngineDisplay();
						ImGui::PopID();
						i++;
					}

					//Add Component
					ImGui::SeparatorText("");

					static float buttonWidth = 15.0f;
					static float size = 15.0f;
					float windowWidth = ImGui::GetWindowSize().x;
					float offset = (windowWidth - buttonWidth) * 0.5f;
					//ImGui::DragFloat("offset", &buttonWidth);
					//ImGui::DragFloat("size", &size);
					ImGui::SetCursorPosX(offset);

					if (ImGui::ImageButton(TextureReader::GetReadTexture(TextureReader::PLUS_T), ImVec2(size, size))) { ImGui::OpenPopup("Components"); }
					if (ImGui::BeginPopup("Components"))
					{
						bool selected = false;

						ImGui::SeparatorText("Components");
						if (ImGui::Selectable("Animator")) { selected = true; vector[index]->AddComponent<Animator>(); }
						if (ImGui::Selectable("AudioListener")) { selected = true; vector[index]->AddComponent<AudioListener>(); }
						if (ImGui::Selectable("AudioSource")) { selected = true; vector[index]->AddComponent<AudioSource>(); }
						if (ImGui::Selectable("Rigidbody")) { selected = true; vector[index]->AddComponent<Rigidbody>(); }
						if (ImGui::Selectable("SphereCollider")) { selected = true; vector[index]->AddComponent<SphereCollider>(); }
						if (ImGui::Selectable("BoxCollider")) { selected = true; vector[index]->AddComponent<BoxCollider>(); }
						if (ImGui::Selectable("MeshFilter")) { selected = true; vector[index]->AddComponent<MeshFilter>(); }
						if (ImGui::Selectable("ParticleSystem")) { selected = true; vector[index]->AddComponent<ParticleSystem>(); }
						if (ImGui::Selectable("MapCollision")) { selected = true; vector[index]->AddComponent<MapCollision>(); }

						if (selected == true)
						{
							Manager::GetScene()->componentAdder.push_back(
								AddComponentSaveFile(
									vector[index]->GetComponentList().back()->name.c_str(),
									vector[index]->GetTag().c_str()
								)
							);
						}

						ImGui::EndPopup();
					}
				}
			}

			ImGui::End();
		}

		//Edit Mode
		{
			ImGui::Begin("Edit Mode", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);

			int ret = edit;
			ImGui::RadioButton("Move", &ret, 0); ImGui::SameLine();
			ImGui::RadioButton("Rotate", &ret, 1); ImGui::SameLine();
			ImGui::RadioButton("Scale", &ret, 2); ImGui::SameLine();

			edit = (EDIT_MODE)ret;

			ImGui::End();
		}

		//Execute Controls
		{
			ImGui::Begin("___", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize/* | ImGuiWindowFlags_NoCollapse*/);

			//ImGui::DragFloat("size", &s, 0.01f);

			ImVec2 size(17.32f, 17.32f);

			//Play Button
			{
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

				if (play == true)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
				}

				if (ImGui::ImageButton(TextureReader::GetReadTexture(TextureReader::PLAY_BUTTON_T), size) || Input::GetKeyTrigger(VK_F5))
				{
					if (play == false)
					{
						Time::timeScale = 1.0f;
						play = true;
						if (cursorOverride == false) { ShowCursor(false); }
					}
					else
					{
						Time::timeScale = 0.0f;
						play = false;
						Manager::ResetScene();
						if (cursorOverride == false) { ShowCursor(true); }
					}
				}

				if (pressedDebug[0] == true) { ImGui::PopStyleColor(2); }

				if (play == true && pressedDebug[0] == false) { pressedDebug[0] = true; }
				if (play == false && pressedDebug[0] == true) { pressedDebug[0] = false; }

				ImGui::PopStyleColor(2);
				ImGui::SameLine();
			}

			//Pause Button
			{
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

				if (paused == true)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
				}

				if (ImGui::ImageButton(TextureReader::GetReadTexture(TextureReader::PAUSE_BUTTON_T), size) || Input::GetKeyTrigger(VK_F6))
				{
					if (paused == true)
					{
						if (play == true) { Time::timeScale = 1.0f; }
						paused = false;
						if (cursorOverride == false) { ShowCursor(false); }
					}
					else
					{
						if (play == true) { Time::timeScale = 0.0f; }
						paused = true;
						if (cursorOverride == false) { ShowCursor(true); }
					}
				}

				if (pressedDebug[1] == true) { ImGui::PopStyleColor(2); }

				if (paused == true && pressedDebug[1] == false) { pressedDebug[1] = true; }
				if (paused == false && pressedDebug[1] == true) { pressedDebug[1] = false; }

				ImGui::PopStyleColor(2);
				ImGui::SameLine();
			}

			//Next Frame Button
			{
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

				if (ImGui::ImageButton(TextureReader::GetReadTexture(TextureReader::NEXT_FRAME_BUTTON_T), size) || Input::GetKeyTrigger(VK_F7))
				{
					if (play == true)
					{
						if (paused == true)
						{
							Time::timeScale = 1.0f;
						}
					}
				}
				else
				{
					if (play == true)
					{
						if (paused == true)
						{
							Time::timeScale = 0.0f;
						}
					}
				}

				ImGui::PopStyleColor(2);
			}

			ImGui::End();
		}

		//Editor
		{
			ImGui::Begin("Editor"/*, nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize*/);

			ImGui::DragFloat("size", &s, 0.05f);
			ImVec2 size(s, s);

			if (ImGui::ImageButton(TextureReader::GetReadTexture(TextureReader::CUBE_T), size))
			{
				Manager::GetScene()->AddGameObject<CUBE>(ObjectIndex("Cube(Clone)"));
				Manager::GetScene()->objectAdder.push_back(AddObjectSaveFile("CUBE", 1));
				layer = GAMEOBJECT_LAYER;
				vector = scene->GetGameObjectListVector((LAYER)layer);
				index = (int)vector.size() - 1;
			}

			ImGui::SameLine();

			if (ImGui::ImageButton(TextureReader::GetReadTexture(TextureReader::CYLINDER_T), size))
			{
				Manager::GetScene()->AddGameObject<CYLINDER>(ObjectIndex("Cylinder(Clone)"));
				Manager::GetScene()->objectAdder.push_back(AddObjectSaveFile("CYLINDER", 1));
				layer = GAMEOBJECT_LAYER;
				vector = scene->GetGameObjectListVector((LAYER)layer);
				index = (int)vector.size() - 1;
			}

			ImGui::SameLine();

			if (ImGui::ImageButton(TextureReader::GetReadTexture(TextureReader::SPRITE_T), size))
			{
				Manager::GetScene()->AddGameObject<IMAGE>(ObjectIndex("Sprite(Clone)"), SPRITE_LAYER);
				Manager::GetScene()->objectAdder.push_back(AddObjectSaveFile("IMAGE", 1));
				layer = SPRITE_LAYER;
				vector = scene->GetGameObjectListVector((LAYER)layer);
				index = (int)vector.size() - 1;
			}

			if (ImGui::ImageButton(TextureReader::GetReadTexture(TextureReader::BILLBOARD_T), size))
			{
				Manager::GetScene()->AddGameObject<BILLBOARD>(ObjectIndex("Billboard(Clone)"), BILLBOARD_LAYER);
				Manager::GetScene()->objectAdder.push_back(AddObjectSaveFile("BILLBOARD", 1));
				layer = BILLBOARD_LAYER;
				vector = scene->GetGameObjectListVector((LAYER)layer);
				index = (int)vector.size() - 1;
			}

			ImGui::SameLine();

			if (ImGui::ImageButton(TextureReader::GetReadTexture(TextureReader::PARTICLE_SYSTEM_T), size))
			{
				Manager::GetScene()->AddGameObject<PARTICLESYSTEM>(ObjectIndex("ParticleSystem(Clone)"), BILLBOARD_LAYER);
				Manager::GetScene()->objectAdder.push_back(AddObjectSaveFile("PARTICLESYSTEM", 1));
				layer = BILLBOARD_LAYER;
				vector = scene->GetGameObjectListVector((LAYER)layer);
				index = (int)vector.size() - 1;
			}

			ImGui::SameLine();

			if (ImGui::ImageButton(TextureReader::GetReadTexture(TextureReader::EMPTY_T), size))
			{
				Manager::GetScene()->AddGameObject<EMPTYOBJECT>(ObjectIndex("EmptyObject(Clone)"));
				Manager::GetScene()->objectAdder.push_back(AddObjectSaveFile("EMPTYOBJECT", 1));
				layer = GAMEOBJECT_LAYER;
				vector = scene->GetGameObjectListVector((LAYER)layer);
				index = (int)vector.size() - 1;
			}

			if (ImGui::ImageButton(TextureReader::GetReadTexture(TextureReader::ENEMY_SPRITE_T), size))
			{
				Manager::GetScene()->AddGameObject<ENEMY>(ObjectIndex("Enemy(Clone)"));
				Manager::GetScene()->objectAdder.push_back(AddObjectSaveFile("ENEMY", 1));
				layer = GAMEOBJECT_LAYER;
				vector = scene->GetGameObjectListVector((LAYER)layer);
				index = (int)vector.size() - 1;
			}

			ImGui::SameLine();

			if (ImGui::ImageButton(TextureReader::GetReadTexture(TextureReader::COLLISION_OBJECT_T), size))
			{
				Manager::GetScene()->AddGameObject<MAPCOLLISIONOBJECT>(ObjectIndex("CollisionObject(Clone)"), MAP_LAYER);
				Manager::GetScene()->objectAdder.push_back(AddObjectSaveFile("MAPCOLLISIONOBJECT", 1));
				layer = MAP_LAYER;
				vector = scene->GetGameObjectListVector((LAYER)layer);
				index = (int)vector.size() - 1;
			}

			ImGui::End();
		}

		//ImGui::Begin("Renderer");

		//static float value[180];
		//for (int i = 0; i < 179; i++) { value[i] = value[i + 1]; }
		//value[179] = ImGui::GetIO().DeltaTime * 1000.0f;
		//ImGui::PlotLines("", value, sizeof(value) / sizeof(float), 0, NULL, 0.0f, 100.0f, ImVec2(0, 50));

		//ImGui::End();
#endif
	}
}

bool DebugManager::BoolDisplay(bool* value, float offset, const char* text, int index, bool uneditable)
{
#ifdef DEBUG

	int reference = (*value ? 1 : 0);

	ImGui::PushItemWidth(offset);
	ImGui::PushID(index);

	if (*value == false) 
	{ 
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); 
		ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
	}
	else 
	{ 
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
	}

	ImGui::SliderInt("", &reference, 0, 1, text);

	if (uneditable == false) { *value = (reference == 0 ? false : true); }

	ImGui::PopStyleColor(2);
	ImGui::PopID();

	return *value;

#else
	return false;
#endif
}

float DebugManager::FloatDisplay(float* value, float offset, const char* text, bool drag, D3DXVECTOR2 speedLimit, int index, bool uneditable)
{
#ifdef DEBUG

	char str[40];
	sprintf_s(str, sizeof(str), "%s : %.2f", text, *value);

	float reference = *value;

	ImGui::PushItemWidth(offset);
	ImGui::PushID(index);

	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.0f, 0.0f, 0.8f, 1.0f));

	if (drag == false) { ImGui::SliderFloat("", &reference, speedLimit.x, speedLimit.y, str); }
	else { ImGui::DragFloat("", &reference, speedLimit.x, 0.0F, 0.0F, str); }

	if (uneditable == false) { *value = reference; }

	ImGui::PopStyleColor(2);
	ImGui::PopID();

	return *value;

#else
	return 0.0f;
#endif
}

D3DXVECTOR3 DebugManager::Float3Display(D3DXVECTOR3* value, float offset, const char* text, float speed, int index, bool uneditable, float min, float max)
{
#ifdef DEBUG

	D3DXVECTOR3 reference = *value;

	ImGui::PushItemWidth(offset);
	ImGui::Text(text);
	ImGui::SameLine();

	ImGui::PushID(index);

	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.0f, 0.0f, 0.8f, 1.0f));

	ImGui::DragFloat3("", reference, speed, min, max);

	if (uneditable == false) { *value = reference; }

	ImGui::PopStyleColor(2);
	ImGui::PopID();

	return *value;

#else
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
#endif
}