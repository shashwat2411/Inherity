#pragma once
#include <algorithm>

#include "prefab.h"

#define GAME_OBJECT_MAX 64 

static bool fcmp(GAMEOBJECT* a, GAMEOBJECT* b) { return a->cameraDistance > b->cameraDistance; }

class COLLISION
{
private:

public:

	void Init();
	void Update();
};

enum LAYER
{
	CAMERA_LAYER = 0,
	GAMEOBJECT_LAYER,
	COLLIDER_LAYER,
	BILLBOARD_LAYER,
	SHADOW_LAYER,
	SPRITE_LAYER,
	FADE_LAYER,
	MAX_LAYER
};

class SCENE
{
protected:
	bool end;
	bool paused;

	CAMERA* MainCamera;
	FADE* Fade;
	SKYDOME* skyDome;

	COLLISION Collision;

	std::array<std::list<GAMEOBJECT*>, MAX_LAYER> GameObjects;

public:
	PLAYER* player;
	GAMEOBJECT* torus;

public:

	//Functions
	void InitBefore();
	void Uninit();
	void UpdateBefore();
	void Draw();
	void FakeDraw(GAMEOBJECT* var);
	void DepthPath();
	void EnvironmentMap();
	void ReflectionMap(D3DXMATRIX* view);

	//Virtual Functions
	virtual void Init() {}
	virtual void Update() {}

	bool GetPaused() { return paused; }
	PLAYER* GetPlayer() { return player; }
	CAMERA* GetCamera() { return (CAMERA*)MainCamera; }
	std::list<GAMEOBJECT*> GetGameObjectList(LAYER layer) { return GameObjects[layer]; }

	void SetEnd() { end = true; }


	//ADD
	template <typename T>
	T* AddGameObject(LAYER layer = GAMEOBJECT_LAYER)
	{
		T* gameObject = new T();
		GameObjects[layer].push_back(gameObject);
		gameObject->Start();

		return gameObject;
	}

	//REMOVE
	template<typename T>
	void RemoveGameObject(LAYER layer)
	{
		T* buff = FindGameObject<T>();
		if (buff != nullptr)
		{
			buff->UnInitialize();
			GameObjects[layer].remove(buff);
			delete buff;
		}
	}

	//FIND
	template<typename T>
	T* FindGameObject()
	{
		for (int i = 0; i < MAX_LAYER; i++)
		{
			for (auto com : GameObjects[i])
			{
				T* buff = dynamic_cast<T*>(com);
				if (buff != nullptr)
				{
					return buff;
				}
			}
		}
		return nullptr;
	}

	//FIND MULTIPLE
	template<typename T>
	std::vector<T*> FindGameObjects()
	{
		std::vector<T*> objects;
		for (int i = 0; i < MAX_LAYER; i++)
		{
			for (auto obj : GameObjects[i])
			{
				T* buff = dynamic_cast<T*>(obj);
				if (buff != nullptr)
				{
					objects.push_back((T*)buff);
				}
			}
		}
		return objects;
	}

	//FIND MULTIPLE
	template<typename T>
	std::vector<T*> FindGameObjectsOfType()
	{
		std::vector<T*> objects;
		for (int i = 0; i < MAX_LAYER; i++)
		{
			for (auto obj : GameObjects[i])
			{
				T* buff = obj->GetComponent<T>();
				if(buff != nullptr)
				{
					objects.push_back(buff);
				}
			}
		}
		return objects;
	}
};