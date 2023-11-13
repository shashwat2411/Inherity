#pragma once
#include "component.h"
#include "../PRIVATE PATTERN/modelReader.h"


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

		AddComponent<MeshFilter>()->SetModel(ModelReader::GetReadModel(ModelReader::SPHERE_COLLIDER_M));

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
	Audio* audio;

public:

	void Init() override
	{
		Initialize();

		ignorePause = true;
	}
	void Update() override
	{
		if (audio != nullptr)
		{
			if (audio->IsPlaying() == false)
			{
				Destroy(true);
			}
		}
	}

};
class CYLINDER : public GAMEOBJECT
{
private:

public:

	void Init() override
	{
		Initialize();

		AddComponent<MeshFilter>()->SetModel(ModelReader::GetReadModel(ModelReader::CYLINDER_M));
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

		AddComponent<MeshFilter>()->SetModel(ModelReader::GetReadModel(ModelReader::CUBE_M));
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

		camera = AddComponent<ThirdPersonCamera>();
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

		ignorePause = true;

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

		GetMaterial()->SetTexture("_Texture", TextureReader::GetReadTexture(TextureReader::NUMBER_T));

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
	float setLife;
	float speed;

	D3DXVECTOR3 rotationDirection;
	D3DXVECTOR3 velocity;

public:

	void Init() override
	{
		Initialize();

		depth = true;

		counter = 0.0f;
		life = 0.0f;
		setLife = 0.0f;
		speed = 0.1f;

		rotationDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		transform->Scale = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
		transform->Position.y = 0.0f;

		billboard = AddComponent<Billboard>();
	}
	void Update() override {}

	void ReInitialize()
	{
		transform->Scale = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
		transform->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		D3DXVECTOR3 direction;
		direction.x = (float)(rand() % 10);
		direction.y = (float)(rand() % 10);
		direction.z = (float)(rand() % 10);
		int dirx = rand() % 2;
		int diry = rand() % 2;
		int dirz = rand() % 2;
		if (dirx == 1) { direction.x *= -1.0f; }
		if (diry == 1) { direction.y *= -1.0f; }
		if (dirz == 1) { direction.z *= -1.0f; }
		D3DXVec3Normalize(&direction, &direction);

		velocity = direction * speed;
		life = (float)(rand() % ((int)FRAME_RATE + 1)) / FRAME_RATE;
	}

};
class FADE : public GAMEOBJECT
{
private:
	bool fadeIn;
	bool fadeOut;

	float alpha;
	float speed;

public:

	void Init() override;
	void Update() override;

	bool GetFadeIn() { return fadeIn; }

	void SetFadeIn(bool value = true) { fadeIn = value; }

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

		GetMaterial()->SetTexture("_Texture", TextureReader::GetReadTexture(TextureReader::SHADOW_T));
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

	void Init() override
	{
		Initialize();

		amount = 0.0f;
		maxAmount = 100.0f;

		transform->Position = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50.0f, 0.0f);
		transform->Scale = D3DXVECTOR3(1.5f, 0.15f, 0.5f);

		sprite = AddComponent<SpriteRenderer>();

		SetBarType();
	}
	void Update() override
	{
		if (Input::GetKeyPress('Q')) { Amount(0.5f); }
		if (Input::GetKeyPress('E')) { Amount(-0.5f); }

		if (type == RIGHT)
		{
			sprite->barOffsetRight = ((maxAmount - amount) / maxAmount) * -(sprite->GetSize().x * transform->Scale.x * 2.0f);
		}
		else if (type == LEFT)
		{
			sprite->barOffsetLeft = ((maxAmount - amount) / maxAmount) * (sprite->GetSize().x * transform->Scale.x * 2.0f);
		}
		else if (type == MIDDLE)
		{
			sprite->barOffsetRight = ((maxAmount - amount) / maxAmount) * -(sprite->GetSize().x * transform->Scale.x);
			sprite->barOffsetLeft = ((maxAmount - amount) / maxAmount) * (sprite->GetSize().x * transform->Scale.x);
		}

#ifdef DEBUG	// デバッグ情報を表示する
		char* str = GetDebugStr();
		//sprintf(&str[strlen(str)], " | Amount : %.2f", amount);
		//sprintf(&str[strlen(str)], " | barOffsetRight : %.2f", sprite->barOffsetRight);
		//sprintf(&str[strlen(str)], " | barOffsetLeft : %.2f", sprite->barOffsetLeft);
#endif
	}

	void SetBarType(BAR_TYPE value = RIGHT)
	{
		type = value;

		if (type == RIGHT)
		{
			sprite->barOffsetRight = -(sprite->GetSize().x * transform->Scale.x) * 2.0f;
		}
		else if (type == LEFT)
		{
			sprite->barOffsetLeft = (sprite->GetSize().x * transform->Scale.x) * 2.0f;
		}
		else if (type == MIDDLE)
		{
			sprite->barOffsetRight = -(sprite->GetSize().x * transform->Scale.x);
			sprite->barOffsetLeft = (sprite->GetSize().x * transform->Scale.x);
		}
	}
	void Amount(float value)
	{
		if (value >= 0.0f)
		{
			if ((amount + value) <= maxAmount) { amount += value; }
			else { amount = maxAmount; }
		}
		else if (value < 0.0f)
		{
			if ((amount + value) >= 0.0f) { amount += value; }
			else { amount = 0.0f; }
		}
	}

};