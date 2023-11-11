#pragma once
#include "../LOAD FUNCTIONS/animationModel.h"
#include "../LOAD FUNCTIONS/model.h"

class ModelReader
{
public:

	enum READ_MODEL_OBJ
	{
		//BASE MODELS
		SPHERE_COLLIDER_M,
		CYLINDER_M,
		CUBE_M,

		//PREFAB MODELS
		SKYDOME_M,
		ENEMY_M,

		READ_MODEL_OBJ_MAX
	};

	enum READ_MODEL_FBX
	{
		//PREFAB MODELS
		THE_BOSS_M,
		ROCK_M,

		READ_MODEL_FBX_MAX
	};


private:
	static Model ModelsOBJ[ModelReader::READ_MODEL_OBJ_MAX];
	static AnimationModel ModelsFBX[ModelReader::READ_MODEL_FBX_MAX];
	static std::unordered_map<std::string, const aiScene*> m_Animation;

public:
	static void ReadModel()
	{
		//OBJ
		ModelsOBJ[SPHERE_COLLIDER_M].Load("asset\\model\\SphereCollider.obj");
		ModelsOBJ[CYLINDER_M].Load("asset\\model\\cylinder.obj");
		ModelsOBJ[CUBE_M].Load("asset\\model\\Cube1.obj");

		ModelsOBJ[SKYDOME_M].Load("asset\\model\\skyDome.obj");
		ModelsOBJ[ENEMY_M].Load("asset\\model\\Explode.obj");


		//FBX
		ModelsFBX[THE_BOSS_M].Load("asset\\model\\The Boss.fbx");
		ModelsFBX[ROCK_M].Load("asset\\model\\rock.fbx");


		//ANIMATION
		m_Animation["Run"] = aiImportFile("asset\\model\\Bot_Run.fbx", aiProcess_ConvertToLeftHanded);
		m_Animation["Idle"] = aiImportFile("asset\\model\\Bot_Idle.fbx", aiProcess_ConvertToLeftHanded);
		m_Animation["Dance"] = aiImportFile("asset\\model\\Twist Dance.fbx", aiProcess_ConvertToLeftHanded);
		m_Animation["Jump"] = aiImportFile("asset\\model\\Standing Jump.fbx", aiProcess_ConvertToLeftHanded);

		assert(m_Animation["Run"]);
		assert(m_Animation["Idle"]);
		assert(m_Animation["Dance"]);
		assert(m_Animation["Jump"]);
	}

	static void UnReadModel()
	{
		for (int i = 0; i < READ_MODEL_OBJ::READ_MODEL_OBJ_MAX; i++)
		{
			ModelsOBJ[i].Unload();
		}

		for (int i = 0; i < READ_MODEL_FBX::READ_MODEL_FBX_MAX; i++)
		{
			ModelsFBX[i].Unload();
		}
	}
	static Model* GetReadModel(READ_MODEL_OBJ value)
	{
		return &ModelsOBJ[value];
	}
	static AnimationModel* GetReadModel(READ_MODEL_FBX value)
	{
		return &ModelsFBX[value];
	}
	static std::unordered_map<std::string, const aiScene*> GetAnimation()
	{
		return m_Animation;
	}

};