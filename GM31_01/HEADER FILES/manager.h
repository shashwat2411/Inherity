#pragma once
#include "scene.h"

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

	static void Load();
	static void Unload();

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

		Scene->LateInit();
	}

	static void ResetScene()
	{
		Scene->Uninit();

		Scene->BeforeInit();
		Scene->Init();

		Open(Scene->name);
	}

};

std::string ObjectIndex(std::string name);