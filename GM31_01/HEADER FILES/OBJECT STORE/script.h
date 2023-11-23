#pragma once
#include "component.h"
#include "modelReader.h"

#define COLLIDE_COUNTDOWN (5.0f / FRAME_RATE)

class PlayerMovement : public Script
{
public:
	enum PLAYER_STATE
	{
		GROUND_PS,
		JUMP_PS,

		PS_MAX
	};
	bool jump;
	bool move;

	int idleCounter;

	D3DXVECTOR3 rotationDirection;

private:
	PLAYER_STATE playerState;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	void UpdateGround();
	void UpdateJump();
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