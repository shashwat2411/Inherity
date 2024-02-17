#pragma once
#include "../saveFunctions.h"

#define COLLIDE_COUNTDOWN (5.0f / FRAME_RATE)
#define SPRING_NUMS	(TILES*(TILES + 1) + TILES*(TILES + 1) + TILES*TILES * 2)	// スプリング総数

class RevolutionCamera;

class PlayerMovement : public Script
{
public:
	enum PLAYER_STATE
	{
		NORMAL_MOVE_PS,
		AIMING_MOVE_PS,
		ROLL_PS,
		DEATH_PS,
		HIT_PS,

		PS_MAX
	};
	bool move;
	bool diagonal;
	bool aim;
	bool gunSelection;

	int idleCounter;

	D3DXVECTOR3 rotationDirection;
	D3DXVECTOR3 direction;
	
	D3DXVECTOR3* aimPoint;

	GAMEOBJECT* gun1;
	GAMEOBJECT* gun2;

	MeshFilter* model;
	Camera* camera;
	RevolutionCamera* cameraController;

private:
	bool setAnimation;

	PLAYER_STATE playerState;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	void SetState(PLAYER_STATE value) { playerState = value; }

	PlayerMovement::PLAYER_STATE GetState() { return playerState; }

	void NormalMove();
	void AimingMove();
	void Roll();
	void Death();
	void Hit();

};
class Ground : public Script
{
public:
	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;
};
class EnemyScript : public Script
{
private:
	bool death;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	void Death();
};
class BulletScript : public Script
{
private:
	bool destruction;

public:

	void Start() override;
	void Update() override;

	void EngineDisplay() override;

	void OnCollisionEnter(GAMEOBJECT* obj) override;

	void OnDestruction(bool enemyCollision);

};
class HitPoints : public Script
{
private:
	bool disable;
	bool invincible;
	bool noDamage;

	float hp;
	float* barController;

	GAMEOBJECT* object;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	float GetHP() { return hp; }
	bool GetNoDamage() { return noDamage; }
	bool GetInvincible() { return invincible; }

	void SetDisable(bool value) { disable = value; }
	void SetHP(float value) { hp = value; }
	void SetBarController(float* value) { barController = value; }
	void SetObject(GAMEOBJECT* value) { object = value; }

	void Invincible() { noDamage = true; invincible = true; }
	void Vulnerable() { noDamage = false; invincible = false; }

	bool Damage(float damage = 1.0f);
	void Heal(float value = 1.0f);
};
class PlayerControl : public Script
{
private:

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

private:
	void Spawn();
};
class GameManager : public Script
{
private:
	GAMEOBJECT* Player;
	float slowMotionSpeed;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

};
class ClothSimulator : public Script
{
public:
	struct WINDFORCE {
		int			status;				// ステータス
		D3DXVECTOR3	force;				// 風力ベクトル
	};
	struct Particle {
		bool	onLock;				// 固定フラグ
		bool	onDrag;				// マウスドラッグ中判定フラグ
		D3DXVECTOR3	position;			// 位置座標 (Position)
		D3DXVECTOR3	velocity;			// 速度 (Velocity)
		D3DXVECTOR3	acceleration;		// 加速度 (Acceleration)
		D3DXVECTOR3	resultant;			// 合力 (Resultant)
		D3DXVECTOR3	gravity;			// 重力 (Gravity)
	};
	struct P_REF {
		int		horz;				// 横方向インデックス
		int		vert;				// 縦方向インデックス
	};
	struct Spring {
		P_REF	p1;					// 接続されている粒子１
		P_REF	p2;					// 接続されている粒子２
		float	length;				// 自然長
	};
	struct CLOTH {
		int			status;														// ステータス
		Particle	particle[TILES + 1][TILES + 1];		// 構成粒子
		Spring		Spring[SPRING_NUMS];										// 粒子間のバネ
	};

public:
	int num;
	MeshField* mesh;
	CLOTH cloth;
	WINDFORCE	windForce;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	void WindForce();
	void ClothSimulation();
};
class ArtificialIntelligence : public Script
{
public:

	enum ENEMY_STATE
	{
		ROAM,
		FOLLOW,
		WAIT,
		ATTACK,
		FIND,
		RETURN,
		DEATH,
		VICTORY,

		ENEMY_STATE_MAX
	};

private:
	bool flip;
	bool lock;

	int index;
	int nextIndex;
	int danceIndex;

	float distance;

	D3DXVECTOR3 startPosition;
	D3DXVECTOR3 returnPosition;

	ENEMY_STATE state;

	Projector* projector;
	MeshFilter* model;
	GAMEOBJECT* target;
	GAMEOBJECT* seeker;
	GAMEOBJECT* attackDistance;

	std::string targetName;
	std::vector<GAMEOBJECT*> points;

public:

	ArtificialIntelligence() { name = "ArtificialIntelligence"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;
	void OnCollisionEnter(GAMEOBJECT* obj) override;

	void SetState(ENEMY_STATE value) { state = value; }

	ArtificialIntelligence::ENEMY_STATE GetState() { return state; }

	void Roam();
	void Follow();
	void Return();
	void Wait();
	void Attack();
	void Find();
	void Death();
	void Victory();

	void Finder();
	void Dancing();

	void SetStateToReturn();

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(
			cereal::virtual_base_class<Component>(this),
			CEREAL_NVP(lock),
			CEREAL_NVP(flip),
			CEREAL_NVP(distance),
			cereal::make_nvp("followDistance", timerVector["followDistance"])
		);
	}
};
class ScreenToWorld : public Script
{
private:
	float speed;
	float aimSpeed;
	D3DXVECTOR2 screenPosition;
	D3DXVECTOR3 worldPosition;
	GAMEOBJECT* point;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	D3DXVECTOR3* CalcScreenToWorld(
		D3DXVECTOR3* pout,
		int Sx,  // スクリーンX座標
		int Sy,  // スクリーンY座標
		float fZ,  // 射影空間でのZ値（0～1）
		int Screen_w,
		int Screen_h,
		D3DXMATRIX* View,
		D3DXMATRIX* Prj
	);

	D3DXVECTOR3* CalcScreenToXZ(
		D3DXVECTOR3* pout,
		int Sx,
		int Sy,
		int Screen_w,
		int Screen_h,
		D3DXMATRIX* View,
		D3DXMATRIX* Prj
	);

	D3DXVECTOR3* GetPoint() { return &point->transform->Position; }
};
class MapCollision : public Script
{
private:
	bool ignore;
	float ignoreSize;

public:

	MapCollision() { name = "MapCollision"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(
			cereal::virtual_base_class<Component>(this),
			CEREAL_NVP(ignore)
		);
	}
};
class GunDissolve : public Script
{
private:
	D3DXCOLOR dissolveColor;

public:

	GunDissolve() { name = "GunDissolve"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

};
class PlayerHealth : public Script
{
private:
	bool invincible;

	float hp;
	float redHp;
	float maxHp;
	float thicknessX;
	float thicknessY;

	BAR* health;

public:

	PlayerHealth() { name = "PlayerHealth"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	bool Damage(float damage);
	void Heal(float heal);

};
class EnemyHealth : public Script
{
private:
	float hp;
	float redHp;
	float maxHp;
	float thicknessX;
	float thicknessY;

	D3DXVECTOR3 offset;

	BILLBOARD* health;

public:

	EnemyHealth() { name = "EnemyHealth"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	void SetOffset(D3DXVECTOR3 value) { offset = value; }
	void SetHealth(float value) { hp = value; redHp = value; maxHp = value; }

	D3DXVECTOR3 GetOffset() { return offset; }
	BILLBOARD* GetHealthObject() { return health; }

	void Damage(float damage);
	void Heal(float heal);
};
class KnifeCollision : public Script
{
private:

public:

	void Start() override;
	void Update() override;

	void EngineDisplay() override;

	void OnCollisionEnter(GAMEOBJECT* obj) override;

};
class MiniMapVariable : public Script
{
private:
	float value;

public:

	void Start() override 
	{
		value = 0.0f;
	}
	void Draw() override 
	{
		FADE* fade = (FADE*)gameObject;
		fade->SetAlpha(value);
	}

	void EngineDisplay() override
	{
		if (ImGui::TreeNode("Mini Map Variable"))
		{
			DebugManager::FloatDisplay(&value, -FLT_MIN, "Alpha", true, D3DXVECTOR2(0.01f, 0.0f), 1);

			ImGui::TreePop();
			ImGui::Spacing();
		}
	}
};
class PauseMenuScript : public Script
{
private:
	D3DXVECTOR3 selectedSize;
	D3DXVECTOR3 originalSize;

public:

	void Start() override;
	void Draw() override;

	void EngineDisplay() override;

};

//Camera Scripts
class CameraScript : public Script
{
protected:
	Camera* camera;

public:

	void Start() { camera = gameObject->GetComponent<Camera>(); }

};
class TopDownCamera : public CameraScript
{
private:

public:

	void Update() override;

	void EngineDisplay() override;

};
class ThirdPersonCamera : public CameraScript
{
private:

public:

	void Update() override;

	void EngineDisplay() override;

};
class FirstPersonCamera : public CameraScript
{
private:

public:

	void Update() override;

	void EngineDisplay() override;

};
class OpenWorldCamera : public CameraScript
{
private:

public:

	void Update() override;

	void EngineDisplay() override;

};
class RevolutionCamera : public CameraScript
{
private:
	float offsetSpeed = 0.02f;
	
	D3DXVECTOR3 screenLimit = D3DXVECTOR3(300.0f, 180.0f, -40.0f);

	D3DXVECTOR3 followSpeed = D3DXVECTOR3(0.012f, 0.08f, 0.0f);
	D3DXVECTOR3 targetOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 backUpDistance = D3DXVECTOR3(0.0f, 4.0f, 8.0f);

public:
	IMAGE* aimer;

public:

	void Update() override;

	void EngineDisplay() override;

	void SetBackUpDistance(D3DXVECTOR3 value) { backUpDistance = value; }
	void SetFollowSpeed(D3DXVECTOR3 value) { followSpeed = value; }
	void SetScreenLimit(D3DXVECTOR3 value) { screenLimit = value; }

	D3DXVECTOR3 GetBackUpDistance() { return backUpDistance; }
	D3DXVECTOR3 GetFollowSpeed() { return followSpeed; }
	D3DXVECTOR3 GetScreenLimit() { return screenLimit; }

};