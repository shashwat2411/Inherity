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

	static void Save();
	static void Open();

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
		Scene->InitBefore();
		Scene->Init();
	}

	static void ResetScene()
	{
		Scene->Uninit();

		Scene->InitBefore();
		Scene->Init();
	}
};
