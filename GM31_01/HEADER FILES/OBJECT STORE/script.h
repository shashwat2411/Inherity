#pragma once
#include "../saveFunctions.h"

#define COLLIDE_COUNTDOWN (5.0f / FRAME_RATE)
#define SPRING_NUMS	(TILES*(TILES + 1) + TILES*(TILES + 1) + TILES*TILES * 2)	// �X�v�����O����

class PlayerMovement : public Script
{
public:
	enum PLAYER_STATE
	{
		NORMAL_MOVE_PS,
		AIMING_MOVE_PS,
		ROLL_PS,

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

private:
	bool setAnimation;

	PLAYER_STATE playerState;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	void NormalMove();
	void AimingMove();
	void Roll();
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

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;
};
class BulletScript : public Script
{
private:

public:

	void Start() override;
	void Update() override;

	void EngineDisplay() override;

	void OnCollisionEnter(GAMEOBJECT* obj) override;

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
		int			status;				// �X�e�[�^�X
		D3DXVECTOR3	force;				// ���̓x�N�g��
	};
	struct Particle {
		bool	onLock;				// �Œ�t���O
		bool	onDrag;				// �}�E�X�h���b�O������t���O
		D3DXVECTOR3	position;			// �ʒu���W (Position)
		D3DXVECTOR3	velocity;			// ���x (Velocity)
		D3DXVECTOR3	acceleration;		// �����x (Acceleration)
		D3DXVECTOR3	resultant;			// ���� (Resultant)
		D3DXVECTOR3	gravity;			// �d�� (Gravity)
	};
	struct P_REF {
		int		horz;				// �������C���f�b�N�X
		int		vert;				// �c�����C���f�b�N�X
	};
	struct Spring {
		P_REF	p1;					// �ڑ�����Ă��闱�q�P
		P_REF	p2;					// �ڑ�����Ă��闱�q�Q
		float	length;				// ���R��
	};
	struct CLOTH {
		int			status;														// �X�e�[�^�X
		Particle	particle[TILES + 1][TILES + 1];		// �\�����q
		Spring		Spring[SPRING_NUMS];										// ���q�Ԃ̃o�l
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

		ENEMY_STATE_MAX
	};

private:
	bool flip;
	bool lock;

	int index;
	int nextIndex;

	float distance;

	D3DXVECTOR3 startPosition;
	D3DXVECTOR3 returnPosition;

	ENEMY_STATE state;

	Projector* projector;
	MeshFilter* model;
	GAMEOBJECT* target;
	GAMEOBJECT* seeker;

	std::string targetName;
	std::vector<GAMEOBJECT*> points;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;
	void OnCollisionEnter(GAMEOBJECT* obj) override;

	void Roam();
	void Follow();
	void Return();
	void Wait();
	void Attack();
	void Find();

	void Finder();
};
class ScreenToWorld : public Script
{
private:
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
		int Sx,  // �X�N���[��X���W
		int Sy,  // �X�N���[��Y���W
		float fZ,  // �ˉe��Ԃł�Z�l�i0�`1�j
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
	D3DXVECTOR3 targetOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

public:

	void Update() override;

	void EngineDisplay() override;

};