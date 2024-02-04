#pragma once
#include <algorithm>

#include "../saveFunctions.h"

#define GAME_OBJECT_MAX 64 

static bool fcmp(GAMEOBJECT* a, GAMEOBJECT* b) { return a->cameraDistance > b->cameraDistance; }

class COLLISION
{
private:

public:

	static void Update();
};

class AddObjectSaveFile
{
public:
	std::string name;
	int number;

public:

	AddObjectSaveFile() {}
	AddObjectSaveFile(std::string n, int num) :name(n), number(num) {}

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(cereal::make_nvp("class", name), CEREAL_NVP(number));
	}
};

class AddComponentSaveFile
{
public:
	std::string name;
	std::string gameObject;

public:

	AddComponentSaveFile() {}
	AddComponentSaveFile(std::string n, std::string g) :name(n), gameObject(g) {}

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(cereal::make_nvp("component", name), CEREAL_NVP(gameObject));
	}
};

enum LAYER
{
	CAMERA_LAYER = 0,
	GAMEOBJECT_LAYER,
	LATEOBJECT_LAYER,
	COLLIDER_LAYER,
	MAP_LAYER,
	GIZMO_LAYER,
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

	CAMERA* MainCamera;
	FADE* Fade;
	SKYDOME* skyDome;
	EMPTYOBJECT* reflectionProjector;
	EMPTYOBJECT* gameManager;

	std::array<std::vector<std::string>, MAX_LAYER> gameObjectNames;
	std::array<std::list<GAMEOBJECT*>, MAX_LAYER> GameObjects;

public:
	std::string name;

	PLAYER* player;

	std::vector<AddObjectSaveFile> objectAdder;
	std::vector<AddComponentSaveFile> componentAdder;
	std::vector<AddComponentSaveFile> animationAdder;

public:

	//Functions
	void Uninit();
	void UpdateBefore();
	void Draw();
	void FakeDraw(GAMEOBJECT* var);
	void DepthPath();
	void EnvironmentMap();
	void ReflectionMap(D3DXMATRIX* view);

	//Virtual Functions
	virtual void BeforeInit();
	virtual void Init() {}
	virtual void LateInit() {}
	virtual void Update() {}

	PLAYER* GetPlayer() { return player; }
	CAMERA* GetCamera() { return (CAMERA*)MainCamera; }
	EMPTYOBJECT* GetReflector() { return reflectionProjector; }
	EMPTYOBJECT* GetGameManager() { return gameManager; }

	std::array<std::list<GAMEOBJECT*>, MAX_LAYER> GetGameObjectList() { return GameObjects; }
	std::list<GAMEOBJECT*> GetGameObjectList(LAYER layer) { return GameObjects[layer]; }
	std::vector<std::string> GetGameObjectNames(LAYER layer) { return gameObjectNames[layer]; }
	std::vector<GAMEOBJECT*> GetGameObjectListVector(LAYER layer) 
	{
		std::vector<GAMEOBJECT*> vector;
		int i = 0;
		for (auto obj : GameObjects[layer])
		{
			i++;
			vector.push_back(obj);
		}
		if (i == 0) { vector.push_back(nullptr); }
		return vector;
	}

	void SetEnd() { end = true; }
	void SetGameObjectName(GAMEOBJECT* itself, LAYER layer) { gameObjectNames[layer][itself->GetID()] = itself->GetTag(); }


	//ADD
	template <typename T>
	T* AddGameObject(std::string name = "", LAYER layer = GAMEOBJECT_LAYER)
	{
		T* gameObject = new T();
		GameObjects[layer].push_back(gameObject);

		if (name != "") { gameObject->SetTag(name); }

		gameObjectNames[layer].push_back(gameObject->GetTag());

		gameObject->Start();

		gameObject->SetID((int)GameObjects[layer].size() - 1);

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

	//FIND
	GAMEOBJECT* Find(std::string name)
	{
		for (int i = 0; i < MAX_LAYER; i++)
		{
			for (auto obj : GameObjects[i])
			{
				if (obj->GetTag() == name)
				{
					return obj;
				}
			}
		}
		return nullptr;
	}

	GAMEOBJECT* Find(std::string name, LAYER layer)
	{
		for (auto obj : GameObjects[int(layer)])
		{
			if (obj->GetTag() == name)
			{
				return obj;
			}
		}
		return nullptr;
	}

	std::vector<GAMEOBJECT*> FindMultiple(std::string name)
	{
		std::vector<GAMEOBJECT*> objects;
		for (int i = 0; i < MAX_LAYER; i++)
		{
			for (auto obj : GameObjects[i])
			{
				size_t underscorePos = obj->GetTag().find('_');
				
				if ((underscorePos != std::string::npos ? obj->GetTag().substr(0, underscorePos) : obj->GetTag()) == name)
				{
					objects.push_back(obj);
				}
			}
		}
		return objects;
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