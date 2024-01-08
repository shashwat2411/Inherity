#pragma once
#include "component.h"
#include "modelReader.h"

#define COLLIDE_COUNTDOWN (5.0f / FRAME_RATE)
#define SPRING_NUMS	(TILES*(TILES + 1) + TILES*(TILES + 1) + TILES*TILES * 2)	// �X�v�����O����

class PlayerMovement : public Script
{
public:
	enum PLAYER_STATE
	{
		GROUND_PS,
		JUMP_PS,
		LIGHT_ATTACK_PS,
		HEAVY_ATTACK_PS,

		PS_MAX
	};
	bool jump;
	bool move;

	int idleCounter;

	D3DXVECTOR3 rotationDirection;

private:
	bool setAnimation;

	int punchState;

	std::string punchAnimation;

	PLAYER_STATE playerState;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	void UpdateGround();
	void UpdateJump();
	void LightAttack();
	void HeavyAttack();
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
public:
	float maxCounter;

	float velocity;

	D3DXVECTOR3 direction;

	GAMEOBJECT* shooter;

public:

	void Start() override;
	void Update() override;

	void EngineDisplay() override;

	void OnCollisionEnter(GAMEOBJECT* obj) override;

	void Shoot(GAMEOBJECT* st, float life, float speed, D3DXVECTOR3 offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
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

public:

	void Update() override;

	void EngineDisplay() override;

};