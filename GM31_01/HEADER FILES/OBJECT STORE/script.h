#pragma once
#include "component.h"
#include "../PRIVATE PATTERN/modelReader.h"

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
};
class EnemyScript : public Script
{
private:
	D3DXVECTOR3 Position;
public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;
};
class BulletScript : public Script
{
public:
	float counter = 0.0f;
	float maxCounter = 3.0f;

	float velocity;

	D3DXVECTOR3 direction;

	GAMEOBJECT* shooter;

public:

	void Start() override;
	void Update() override;

	void OnCollisionEnter(GAMEOBJECT* obj) override;

	void Shoot(GAMEOBJECT* st, float life, float speed, D3DXVECTOR3 offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
};
class HitPoints : public Script
{
private:
	bool disable;
	bool invincible;
	bool noDamage;

	float invincibilityCounter;
	float hp;
	float* barController;

	GAMEOBJECT* object;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	int GetHP() { return hp; }
	bool GetNoDamage() { return noDamage; }
	bool GetInvincible() { return invincible; }

	void SetDisable(bool value) { disable = value; }
	void SetHP(int value) { hp = value; }
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

private:
	void Spawn();
};
class GameManager : public Script
{
private:
	GAMEOBJECT* Player;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

};