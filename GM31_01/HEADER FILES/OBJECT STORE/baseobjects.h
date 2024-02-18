#pragma once
#include "material.h"


class EMPTYOBJECT : public GAMEOBJECT
{
private:

public:

	void Init() override { Initialize(); }
	void Update() override {}

};
class SPHERECOLLIDER : public GAMEOBJECT
{
public:

	void Init() override
	{
		Initialize();

		AddComponent<MeshFilter>()->SetModel(ModelReader::SPHERE_COLLIDER_M);

		AddMaterial<UnlitMaterial>();


#ifndef DEBUG
		GetComponent<MeshFilter>()->SetEnabled(false);
#endif
	}
	void Update() override {}

};
class SOUNDAREA : public GAMEOBJECT
{
public:

	void Init() override
	{
		Initialize();

		AddComponent<MeshFilter>()->SetModel(ModelReader::SOUND_GIZMO_M);

		AddMaterial<UnlitMaterial>();


#ifndef DEBUG
		GetComponent<MeshFilter>()->SetEnabled(false);
#endif
	}
	void Update() override {}

};
class BOXCOLLIDER : public GAMEOBJECT
{
public:

	void Init() override
	{
		Initialize();

		AddComponent<MeshFilter>()->SetModel(ModelReader::BOX_COLLIDER_M);

		AddMaterial<UnlitMaterial>();


#ifndef DEBUG
		GetComponent<MeshFilter>()->SetEnabled(false);
#endif
	}
	void Update() override {}

};
class SOUND : public GAMEOBJECT
{
public:
	AudioSource* source;

public:

	void Init() override;
	void Update() override;

};
class CYLINDER : public GAMEOBJECT
{
private:

public:

	void Init() override
	{
		Initialize();

		AddComponent<MeshFilter>()->SetModel(ModelReader::CYLINDER_M);
	}
	void Update() override {}

};
class CUBE : public GAMEOBJECT
{
private:

public:

	void Init() override
	{
		Initialize();

		AddComponent<MeshFilter>()->SetModel(ModelReader::CUBE_M);
	}
	void Update() override {}

};
class CAMERA : public GAMEOBJECT
{
public:
	Camera* camera;

public:

	void Init() override
	{
		Initialize();

		camera = AddComponent<Camera>();
	}
	void Update() override {}

};
class BILLBOARD : public GAMEOBJECT
{
public:
	Billboard* billboard;

protected:

public:

	void Init() override
	{
		Initialize();

		depth = true;
		depthShadow = false;

		transform->Scale = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
		transform->Position.y = 1.0f;

		billboard = AddComponent<Billboard>();
	}
	void Update() override {}
	
};
class PLANE :public GAMEOBJECT
{
public:
	MeshField* meshField;

public:

	void Init() override
	{
		Initialize();

		depth = true;

		meshField = AddComponent<MeshField>();
	}
	void Update() override {}

};
class IMAGE :public GAMEOBJECT
{
public:
	SpriteRenderer* sprite;

public:

	void Init() override
	{
		Initialize();

		depthShadow = false;

		sprite = AddComponent<SpriteRenderer>();
	}
	void Update() override {}

};
class DIGIT : public GAMEOBJECT
{
public:

	void Init() override
	{
		Initialize();

		AddComponent<SpriteRenderer>();

		GetMaterial()->SetTexture("_Texture", TextureReader::NUMBER_T);

	}
	void Update() override {}

};
class NUMBER : public GAMEOBJECT
{
private:
	int num;

public:
	Number* number;

public:

	void Init() override
	{
		Initialize();

		num = 0;

		number = AddComponent<Number>();
	}
	void Update() override
	{
		number->SetNumber(num);
		number->SetNumberColor(GetColor());
	}

	int GetNumber() { return num; }

	void SetNumber(int value) { num = value; }
	void SetDigits(int value)
	{
		number->SetDigits(value);
	}

	void Increment(int value = 1) { num += value; }

};
class PARTICLESYSTEM :public GAMEOBJECT
{
public:
	ParticleSystem* particleSystem;

public:

	void Init() override
	{
		Initialize();

		particleSystem = AddComponent<ParticleSystem>();
	}
	void Update() override {}

};
class PARTICLE : public BILLBOARD
{
public:
	float counter;
	float life;

	D3DXVECTOR3 rotationVelocity;
	D3DXVECTOR3 direction;

public:

	void Init() override
	{
		Initialize();

		depth = true;

		counter = 0.0f;
		life = 1.0f;

		rotationVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		direction = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		transform->Scale = D3DXVECTOR3(0.1f, 0.1f, 0.1f);

		billboard = AddComponent<Billboard>();
	}
	void Update() override {}

};
class FADE : public GAMEOBJECT
{
private:
	bool fadeIn;
	bool fadeOut;
	bool soundPlay;

	float alpha;
	float speed;

public:

	void Init() override;
	void Update() override;

	bool GetFadeIn() { return fadeIn; }
	bool GetFadeOut() { return fadeOut; }
	float GetAlpha() { return alpha; }
	float GetSpeed() { return speed; }

	void SetFadeIn(bool value = true) { fadeIn = value; }
	void SetFadeOut(bool value) { fadeOut = value; }
	void SetAlpha(float value) { alpha = value; }

	bool FadeOut();

};
class SKYDOME : public GAMEOBJECT
{
public:

	void Init() override;
	void Update() override;

};
class SHADOW :public GAMEOBJECT
{
public:
	Plane* plane;

public:

	void Init() override
	{
		Initialize();

		depth = true;
		active = false;

		transform->Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		plane = AddComponent<Plane>();
		plane->Size = D3DXVECTOR2(1.0f, 1.0f);

		GetMaterial()->SetTexture("_Texture", TextureReader::SHADOW_T);
	}
	void Update() override {}

};
class BAR :public GAMEOBJECT
{
public:
	enum BAR_TYPE
	{
		RIGHT,
		LEFT,
		MIDDLE
	};

public:
	float amount;
	float maxAmount;

	BAR_TYPE type;

	SpriteRenderer* sprite;

public:

	void Init() override;
	void Update() override;

	void SetBarType(BAR_TYPE value = RIGHT);
	void Amount(float value);

};
class POSTPROCESS :public GAMEOBJECT
{
public:
	PostProcess* poster;

public:

	void Init() override
	{
		Initialize();

		poster = AddComponent<PostProcess>();
	}
	void Update() override {}

};