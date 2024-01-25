	#pragma once
#include "script.h"

class PLAYER : public GAMEOBJECT
{
public:

	void Start() override;
	void Update() override {}

};
class PLAYERMODEL : public GAMEOBJECT
{
private:
	MeshFilter* model;
	SphereCollider* collider;

public:

	void Start() override;
	void Update() override;

};
class ENEMY : public GAMEOBJECT
{
private:

public:

	void Start() override;
	void Update() override {}
};
class BULLET : public GAMEOBJECT
{
public:
	float speed;

private:

public:

	void Start() override;
	void Update() override {}

};
class ROCK : public GAMEOBJECT
{
private:
	float dissolveSpeed;
	float dissolveThreshold;

public:

	void Start() override;
	void Update() override;

};

class EXPLOSION : public BILLBOARD
{
public:

	void Start() override;
	void Update() override {}
};
class TREE : public BILLBOARD
{
public:

	void Start() override;
	void Update() override {}
};