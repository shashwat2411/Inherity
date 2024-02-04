#pragma once
#include "animationModel.h"
#include "model.h"

#define TOMATO_MODEL

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
		TITLE_PLAYER_MODEL_M,
		MAP_M,
		KNIFE_M,
		TOMATO_GUN_M,
		BULLET_M,

		READ_MODEL_OBJ_MAX
	};

	enum READ_MODEL_FBX
	{
		//PREFAB MODELS
		THE_BOSS_M,
		TOMATO_M,
		ROCK_M,
		HUMAN_M,
		MAP_FBX_M,

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
		ModelsOBJ[TITLE_PLAYER_MODEL_M].Load("asset\\model\\titlePlayerModel.obj");
		ModelsOBJ[MAP_M].Load("asset\\model\\Map\\map2.obj");
		ModelsOBJ[KNIFE_M].Load("asset\\model\\Knife.obj");
		ModelsOBJ[TOMATO_GUN_M].Load("asset\\model\\Tomato\\Gun.obj");
		ModelsOBJ[BULLET_M].Load("asset\\model\\Bullet.obj");


		//FBX
		ModelsFBX[THE_BOSS_M].Load("asset\\model\\The Boss.fbx");
		ModelsFBX[TOMATO_M].Load("asset\\model\\Tomato\\tomato.fbx");
		ModelsFBX[ROCK_M].Load("asset\\model\\rock.fbx");
		ModelsFBX[HUMAN_M].Load("asset\\model\\Enemy\\Enemy.fbx");
		ModelsFBX[MAP_FBX_M].Load("asset\\model\\Map\\map.fbx");


		//ANIMATION

		//PLAYER
		//Move
		Animations["Run"] = aiImportFile("asset\\model\\Tomato\\Goofy Running.fbx", aiProcess_ConvertToLeftHanded);
		Animations["Idle"] = aiImportFile("asset\\model\\Tomato\\Mutant Idle.fbx", aiProcess_ConvertToLeftHanded);
		Animations["Walk"] = aiImportFile("asset\\model\\Tomato\\Mutant Walking.fbx", aiProcess_ConvertToLeftHanded);
		Animations["Roll"] = aiImportFile("asset\\model\\Tomato\\Standing Dive Forward.fbx", aiProcess_ConvertToLeftHanded);

		//Aiming
		Animations["Forward_Jog"] = aiImportFile("asset\\model\\Tomato\\Aiming\\Forward Jog.fbx", aiProcess_ConvertToLeftHanded);
		Animations["Backward_Jog"] = aiImportFile("asset\\model\\Tomato\\Aiming\\Backward Jog.fbx", aiProcess_ConvertToLeftHanded);
		Animations["Left_Jog"] = aiImportFile("asset\\model\\Tomato\\Aiming\\Left Jog.fbx", aiProcess_ConvertToLeftHanded);
		Animations["Right_Jog"] = aiImportFile("asset\\model\\Tomato\\Aiming\\Right Jog.fbx", aiProcess_ConvertToLeftHanded);

		Animations["Forward_Left_Jog"] = aiImportFile("asset\\model\\Tomato\\Aiming\\Forward Left Jog.fbx", aiProcess_ConvertToLeftHanded);
		Animations["Forward_Right_Jog"] = aiImportFile("asset\\model\\Tomato\\Aiming\\Forward Right Jog.fbx", aiProcess_ConvertToLeftHanded);
		Animations["Backward_Left_Jog"] = aiImportFile("asset\\model\\Tomato\\Aiming\\Backward Left Jog.fbx", aiProcess_ConvertToLeftHanded);
		Animations["Backward_Right_Jog"] = aiImportFile("asset\\model\\Tomato\\Aiming\\Backward Right Jog.fbx", aiProcess_ConvertToLeftHanded);

		assert(Animations["Run"]);
		assert(Animations["Idle"]);
		assert(Animations["Walk"]);
		assert(Animations["Roll"]);

		assert(Animations["Forward_Jog"]);
		assert(Animations["Backward_Jog"]);
		assert(Animations["Left_Jog"]);
		assert(Animations["Right_Jog"]);

		assert(Animations["Forward_Left_Jog"]);
		assert(Animations["Forward_Right_Jog"]);
		assert(Animations["Backward_Left_Jog"]);
		assert(Animations["Backward_Right_Jog"]);

		//ENEMY
		Animations["Enemy_Walk"] = aiImportFile("asset\\model\\Enemy\\Enemy Walk.fbx", aiProcess_ConvertToLeftHanded);
		Animations["Enemy_Find"] = aiImportFile("asset\\model\\Enemy\\Enemy Find.fbx", aiProcess_ConvertToLeftHanded);
		Animations["Enemy_Attack_1"] = aiImportFile("asset\\model\\Enemy\\Enemy Attack 1.fbx", aiProcess_ConvertToLeftHanded);
		Animations["Enemy_Run"] = aiImportFile("asset\\model\\Enemy\\Enemy Run.fbx", aiProcess_ConvertToLeftHanded);
		Animations["Enemy_Damage"] = aiImportFile("asset\\model\\Enemy\\Enemy Damage.fbx", aiProcess_ConvertToLeftHanded);
		Animations["Enemy_Wait"] = aiImportFile("asset\\model\\Enemy\\Enemy Wait.fbx", aiProcess_ConvertToLeftHanded);
		Animations["Enemy_Death"] = aiImportFile("asset\\model\\Enemy\\Enemy Death.fbx", aiProcess_ConvertToLeftHanded);

		assert(Animations["Enemy_Walk"]);
		assert(Animations["Enemy_Find"]);
		assert(Animations["Enemy_Attack_1"]);
		assert(Animations["Enemy_Run"]);
		assert(Animations["Enemy_Damage"]);
		assert(Animations["Enemy_Wait"]);
		assert(Animations["Enemy_Death"]);

		//NAMING

		//OBJ
		modelNames[SPHERE_COLLIDER_M]	= "SPHERE_COLLIDER";
		modelNames[BOX_COLLIDER_M]		= "BOX_COLLIDER";
		modelNames[CYLINDER_M]			= "CYLINDER";
		modelNames[CUBE_M]				= "CUBE";
		modelNames[TORUS_M]				= "TORUS";
		modelNames[SOUND_GIZMO_M]		= "SOUND_GIZMO";

		modelNames[TITLE_SKYDOME_M]			= "TITLE SKYDOME";
		modelNames[SKYDOME_M]				= "SKYDOME";
		modelNames[ENEMY_M]					= "ENEMY";
		modelNames[TITLE_PLAYER_MODEL_M]	= "TITLE PLAYER MODEL";
		modelNames[MAP_M]					= "MAP";
		modelNames[KNIFE_M]					= "KNIFE";
		modelNames[TOMATO_GUN_M]			= "TOMATO GUN";
		modelNames[BULLET_M]				= "BULLET";

		//FBX
		int i = READ_MODEL_OBJ_MAX;
		modelNames[i + THE_BOSS_M]		= "THE_BOSS";
		modelNames[i + TOMATO_M]		= "TOMATO";
		modelNames[i + ROCK_M]			= "ROCK";
		modelNames[i + HUMAN_M]			= "HUMAN";
		modelNames[i + MAP_FBX_M]		= "MAPFBX";
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