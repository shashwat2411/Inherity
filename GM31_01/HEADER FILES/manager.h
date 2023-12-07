#pragma once
#include "scene.h"

class POSTPROCESS;

class Manager
{
private:
	static POSTPROCESS* PostProcess;
	static SCENE* Scene;
	static SCENE* DontDestroyOnLoad;
	//static SCENE* NextScene;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void FixedUpdate();
	static void Draw();

	static void Save(std::string name);
	static void Open(std::string name);

	static SCENE* GetScene() { return Scene; }
	static SCENE* GetDontDestroyOnLoadScene() { return DontDestroyOnLoad; }

	template<typename T>
	static void SetScene()
	{
		if (Scene)
		{
			Scene->Uninit();
			delete Scene;
		}

		Scene = new T();
		Scene->BeforeInit();
		Scene->Init();

		Open(Scene->name);

		//for (int i = 0; i < MAX_LAYER; i++)
		//{
		//	for (auto obj : Scene->GetGameObjectList((LAYER)i))
		//	{
		//		Animator* anim = obj->GetComponent<Animator>();
		//		if (anim != nullptr)
		//		{
		//			anim->BezierCalculate();
		//		}
		//	}
		//}
	}

	static void ResetScene()
	{
		Scene->Uninit();

		Scene->BeforeInit();
		Scene->Init();

		Open(Scene->name);
		//for (int i = 0; i < MAX_LAYER; i++)
		//{
		//	for (auto obj : Scene->GetGameObjectList((LAYER)i))
		//	{
		//		Animator* anim = obj->GetComponent<Animator>();
		//		if (anim != nullptr)
		//		{
		//			anim->BezierCalculate();
		//		}
		//	}
		//}
	}
};