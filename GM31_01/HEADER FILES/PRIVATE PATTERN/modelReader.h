#pragma once
#include "animationModel.h"
#include "model.h"

class ModelReader
{
public:

	enum READ_MODEL_OBJ
	{
		//BASE MODELS
		SPHERE_COLLIDER_M,
		BOX_COLLIDER_M,
		CYLINDER_M,
		CUBE_M,
		TORUS_M,
		SOUND_GIZMO_M,

		//PREFAB MODELS
		TITLE_SKYDOME_M,
		SKYDOME_M,
		ENEMY_M,

		READ_MODEL_OBJ_MAX
	};

	enum READ_MODEL_FBX
	{
		//PREFAB MODELS
		THE_BOSS_M,
		ROCK_M,
		//ENEM_M,

		READ_MODEL_FBX_MAX
	};


private:
	static std::vector<Model> ModelsOBJ;
	static std::vector<AnimationModel> ModelsFBX;
	//static Model ModelsOBJ[ModelReader::READ_MODEL_OBJ_MAX];
	//static AnimationModel ModelsFBX[ModelReader::READ_MODEL_FBX_MAX];
	static std::unordered_map<std::string, const aiScene*> Animations;

	static std::vector<const char*> modelNames;
	//static const char* modelNames[READ_MODEL_OBJ_MAX + READ_MODEL_FBX_MAX];

public:
	static void ReadModel()
	{
		for (int i = 0; i < READ_MODEL_OBJ_MAX; i++) { ModelsOBJ.emplace_back(); }
		for (int i = 0; i < READ_MODEL_FBX_MAX; i++) { ModelsFBX.emplace_back(); }
		for (int i = 0; i < READ_MODEL_OBJ_MAX + READ_MODEL_FBX_MAX; i++) { modelNames.emplace_back(); }

		//OBJ
		ModelsOBJ[SPHERE_COLLIDER_M].Load("asset\\model\\SphereCollider.obj");
		ModelsOBJ[BOX_COLLIDER_M].Load("asset\\model\\BoxCollider.obj");
		ModelsOBJ[CYLINDER_M].Load("asset\\model\\cylinder.obj");
		ModelsOBJ[CUBE_M].Load("asset\\model\\Cube.obj");
		ModelsOBJ[TORUS_M].Load("asset\\model\\torus.obj");
		ModelsOBJ[SOUND_GIZMO_M].Load("asset\\model\\soundAreaGizmo.obj");

		ModelsOBJ[TITLE_SKYDOME_M].Load("asset\\model\\titleSkydome.obj");
		ModelsOBJ[SKYDOME_M].Load("asset\\model\\skyDome.obj");
		ModelsOBJ[ENEMY_M].Load("asset\\model\\ExplodeNew.obj");


		//FBX
		ModelsFBX[THE_BOSS_M].Load("asset\\model\\The Boss.fbx");
		ModelsFBX[ROCK_M].Load("asset\\model\\rock.fbx");
		//ModelsFBX[ENEM_M].Load("asset\\model\\Explode.fbx");


		//ANIMATION
		Animations["Run"] = aiImportFile("asset\\model\\Bot_Run.fbx", aiProcess_ConvertToLeftHanded);
		Animations["Idle"] = aiImportFile("asset\\model\\Bot_Idle.fbx", aiProcess_ConvertToLeftHanded);
		Animations["Dance"] = aiImportFile("asset\\model\\Twist Dance.fbx", aiProcess_ConvertToLeftHanded);
		Animations["Jump"] = aiImportFile("asset\\model\\Standing Jump.fbx", aiProcess_ConvertToLeftHanded);

		assert(Animations["Run"]);
		assert(Animations["Idle"]);
		assert(Animations["Dance"]);
		assert(Animations["Jump"]);


		//NAMING

		//OBJ
		modelNames[SPHERE_COLLIDER_M]	= "SPHERE_COLLIDER";
		modelNames[BOX_COLLIDER_M]		= "BOX_COLLIDER";
		modelNames[CYLINDER_M]			= "CYLINDER";
		modelNames[CUBE_M]				= "CUBE";
		modelNames[TORUS_M]				= "TORUS";
		modelNames[SOUND_GIZMO_M]		= "SOUND_GIZMO";

		modelNames[TITLE_SKYDOME_M]		= "TITLE SKYDOME";
		modelNames[SKYDOME_M]			= "SKYDOME";
		modelNames[ENEMY_M]				= "ENEMY";

		//FBX
		int i = READ_MODEL_OBJ_MAX;
		modelNames[i + THE_BOSS_M]		= "THE_BOSS";
		modelNames[i + ROCK_M]			= "ROCK";
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

		for (std::pair<const std::string, const aiScene*> pair : Animations)
		{
			if (pair.second != nullptr)
			{
				aiReleaseImport(pair.second);
			}
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
	static const aiScene* GetAnimation(const char* name)
	{
		return Animations[name];
	}
	//static const char* const* GetModelNames()
	//{
	//	return modelNames;
	//}
	static const char* const* GetModelNames()
	{
		const char** names;
		names = &modelNames[0];
		return names;
	}
};