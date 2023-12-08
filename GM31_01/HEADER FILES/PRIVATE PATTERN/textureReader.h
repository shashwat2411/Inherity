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
		PLAY_BUTTON_T,
		PAUSE_BUTTON_T,
		NEXT_FRAME_BUTTON_T,
		CUBE_T,
		CYLINDER_T,

		//MATERIAL TEXTURES
		DISSOLVE_T,
		WIPE_T,
		TOON_T,
		ENVMAP_T,
		ENVCUBEMAP_T,

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
		BUBBLE_T,

		READ_TEXTURE_MAX
	};

private:
	static ID3D11ShaderResourceView* Textures[READ_TEXTURE_MAX];

	static const char* textureNames[TextureReader::READ_TEXTURE_MAX];

public:
	static void ReadTexture()
	{
		LoadTexture(&Textures[NUMBER_T], "asset\\texture\\number.png");
		LoadTexture(&Textures[SHADOW_T], "asset\\texture\\shadow.png");
		LoadTexture(&Textures[FADE_T], "asset\\texture\\fade.png");
		LoadTexture(&Textures[BOX_T], "asset\\texture\\Box.png");
		LoadTexture(&Textures[HOME2_T], "asset\\texture\\home2.png");
		LoadTexture(&Textures[WATER_T], "asset\\texture\\waterTexture.png");
		LoadTexture(&Textures[PLAY_BUTTON_T], "asset\\texture\\imGui\\playButton.png");
		LoadTexture(&Textures[PAUSE_BUTTON_T], "asset\\texture\\imGui\\pauseButton.png");
		LoadTexture(&Textures[NEXT_FRAME_BUTTON_T], "asset\\texture\\imGui\\nextFrameButton.png");
		LoadTexture(&Textures[CUBE_T], "asset\\texture\\imGui\\cube.png");
		LoadTexture(&Textures[CYLINDER_T], "asset\\texture\\imGui\\cylinder.png");

		LoadTexture(&Textures[DISSOLVE_T], "asset\\texture\\dissolve.png");
		LoadTexture(&Textures[WIPE_T], "asset\\texture\\pipo-tr005.png");
		LoadTexture(&Textures[TOON_T], "asset\\texture\\Material_Textures\\toon.png");
		LoadTexture(&Textures[ENVMAP_T], "asset\\texture\\Material_Textures\\EnvironmentalMapping\\envmap.png");
		LoadTexture(&Textures[ENVCUBEMAP_T], "asset\\texture\\Material_Textures\\EnvironmentalMapping\\envmap4.dds");

		LoadTexture(&Textures[FIELD_NM_T], "asset\\texture\\field004Normal.png");

		LoadTexture(&Textures[TREE_T], "asset\\texture\\tree.png");
		LoadTexture(&Textures[RING_T], "asset\\texture\\ring_c.png");
		LoadTexture(&Textures[EXPLOSION_T], "asset\\texture\\explosion.png");
		LoadTexture(&Textures[ROCK_T], "asset\\texture\\field_dds.dds");
		LoadTexture(&Textures[GRASS_T], "asset\\texture\\grass.png");
		LoadTexture(&Textures[GROUND_T], "asset\\texture\\field004.jpg");
		LoadTexture(&Textures[TITLE_BG_T], "asset\\texture\\titlegm.png");
		LoadTexture(&Textures[RESULT_BG_T], "asset\\texture\\resultgm.png");
		LoadTexture(&Textures[BUBBLE_T], "asset\\texture\\bubble.png");


		//NAMES
		textureNames[NUMBER_T]				= "NUMBER";
		textureNames[SHADOW_T]				= "SHADOW";
		textureNames[FADE_T]				= "FADE";
		textureNames[BOX_T]					= "BOX";
		textureNames[HOME2_T]				= "HOME2";
		textureNames[WATER_T]				= "WATER";
		textureNames[PLAY_BUTTON_T]			= "PLAY_BUTTON";
		textureNames[PAUSE_BUTTON_T]		= "PAUSE_BUTTON";
		textureNames[NEXT_FRAME_BUTTON_T]	= "NEXT_FRAME_BUTTON";
		textureNames[CUBE_T]				= "CUBE";
		textureNames[CYLINDER_T]			= "CYLINDER";

		textureNames[DISSOLVE_T]			= "DISSOLVE";
		textureNames[WIPE_T]				= "WIPE";
		textureNames[TOON_T]				= "TOON";
		textureNames[ENVMAP_T]				= "ENVMAP";
		textureNames[ENVCUBEMAP_T]			= "ENVCUBEMAP";

		textureNames[FIELD_NM_T]			= "FIELD_NM";

		textureNames[TREE_T]				= "TREE";
		textureNames[RING_T]				= "RING";
		textureNames[EXPLOSION_T]			= "EXPLOSION";
		textureNames[ROCK_T]				= "ROCK";
		textureNames[GRASS_T]				= "GRASS";
		textureNames[GROUND_T]				= "GROUND";
		textureNames[TITLE_BG_T]			= "TITLE_BG";
		textureNames[RESULT_BG_T]			= "RESULT_BG";
		textureNames[BUBBLE_T]				= "BUBBLE";
	}

	static void UnReadTexture()
	{
		for (int i = 0; i < READ_TEXTURE::READ_TEXTURE_MAX; i++)
		{
			if (Textures[i] != nullptr) 
			{ 
				Textures[i]->Release(); 
			}
		}
	}
	static ID3D11ShaderResourceView* GetReadTexture(READ_TEXTURE value)
	{
		return Textures[value];
	}
	static const char* const* GetTextureNames()
	{
		return textureNames;
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