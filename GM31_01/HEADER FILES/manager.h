#pragma once
#include "../MANAGEMENT FUNCTIONS/scene.h"

class Manager
{
private:
	static SCENE* Scene;
	static SCENE* DontDestroyOnLoad;
	//static SCENE* NextScene;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void FixedUpdate();
	static void Draw();

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
};
