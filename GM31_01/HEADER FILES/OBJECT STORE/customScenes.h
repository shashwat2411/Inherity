#pragma once
#include "manager.h"

class EMPTY_SCENE : public SCENE
{
public:

	void Init() override {}
	void Update() override {}
};
class TITLE_SCENE : public SCENE
{
public:

	void Init() override;
	void LateInit() override;
	void Update() override;
};
class GAME_SCENE : public SCENE
{
public:
	enum PARTICLE_EFFECT
	{
		BULLET_TO_WALL,
		BULLET_TO_ENEMY,
		ENEMY_TO_PLAYER,

		PARTICLE_EFFECT_MAX
	};

public:
	std::array<std::vector<PARTICLESYSTEM*>, PARTICLE_EFFECT_MAX> particleEffect;

public:

	void Init() override;
	void LateInit() override;
	void Update() override;

	PARTICLESYSTEM* GetParticleEffect(GAME_SCENE::PARTICLE_EFFECT layer)
	{
		for (PARTICLESYSTEM* effect : particleEffect[layer])
		{
			if (effect->GetActive() == false)
			{
				return effect;
			}
		}
		return nullptr;
	}
};
class RESULT_SCENE : public SCENE
{
public:

	void Init() override;
	void Update() override;
};
class LOAD_SCENE : public SCENE
{
private:
	static ID3D11ShaderResourceView* texture;
	static ID3D11ShaderResourceView* backTexture;
	static bool loadOver;
	static bool logo;

public:

	void BeforeInit() override;
	void Init() override;
	void Update() override;

	static bool GetLoadOver() { return loadOver; }
	static bool GetLogo() { return logo; }

	static void SetLoadOver(bool value) { loadOver = value; }
};
class GEOMETRY_INSTANCING_SCENE : public SCENE
{
public:

	void Init() override;
	void Update() override;
};