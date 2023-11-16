#pragma once
#include "main.h"
#include "renderer.h"

class TextureReader
{
public:

	enum READ_TEXTURE
	{
		//BASIC TEXTURES
		NUMBER_T,
		SHADOW_T,
		FADE_T,
		BOX_T,
		HOME2_T,
		WATER_T,

		//MATERIAL TEXTURES
		DISSOLVE_T,
		WIPE_T,

		//NORMAL MAP TEXTURES
		FIELD_NM_T,

		//PREFAB TEXTURES
		TREE_T,
		RING_T,
		EXPLOSION_T,
		ROCK_T,
		GRASS_T,
		GROUND_T,
		TITLE_BG_T,
		RESULT_BG_T,

		READ_TEXTURE_MAX
	};

private:
	static ID3D11ShaderResourceView* Textures[READ_TEXTURE_MAX];

public:
	static void ReadTexture()
	{
		LoadTexture(&Textures[NUMBER_T], "asset\\texture\\number.png");
		LoadTexture(&Textures[SHADOW_T], "asset\\texture\\shadow.png");
		LoadTexture(&Textures[FADE_T], "asset\\texture\\fade.png");
		LoadTexture(&Textures[BOX_T], "asset\\texture\\Box.png");
		LoadTexture(&Textures[HOME2_T], "asset\\texture\\home2.png");
		LoadTexture(&Textures[WATER_T], "asset\\texture\\waterTexture.png");

		LoadTexture(&Textures[DISSOLVE_T], "asset\\texture\\dissolve.png");
		LoadTexture(&Textures[WIPE_T], "asset\\texture\\pipo-tr005.png");

		LoadTexture(&Textures[FIELD_NM_T], "asset\\texture\\field004Normal.png");

		LoadTexture(&Textures[TREE_T], "asset\\texture\\tree.png");
		LoadTexture(&Textures[RING_T], "asset\\texture\\ring_c.png");
		LoadTexture(&Textures[EXPLOSION_T], "asset\\texture\\explosion.png");
		LoadTexture(&Textures[ROCK_T], "asset\\texture\\field_dds.dds");
		LoadTexture(&Textures[GRASS_T], "asset\\texture\\grass.png");
		LoadTexture(&Textures[GROUND_T], "asset\\texture\\field004.jpg");
		LoadTexture(&Textures[TITLE_BG_T], "asset\\texture\\titlegm.png");
		LoadTexture(&Textures[RESULT_BG_T], "asset\\texture\\resultgm.png");
	}

	static void UnReadTexture()
	{
		for (int i = 0; i < READ_TEXTURE::READ_TEXTURE_MAX; i++)
		{
			Textures[i]->Release();
		}
	}
	static ID3D11ShaderResourceView* GetReadTexture(READ_TEXTURE value)
	{
		return Textures[value];
	}

	static void LoadTexture(ID3D11ShaderResourceView** texture, const char* path)
	{
		//テクスチャ読み込み
			D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
				path,
				NULL,
				NULL,
				texture,
				NULL);
		assert(texture);
	}
};