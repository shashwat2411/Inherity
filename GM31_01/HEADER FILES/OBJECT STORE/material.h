#pragma once
#include <unordered_map>

#include "gameobject.h"
#include "textureReader.h"
#include "functions.h"

class Material
{
protected:
	bool reflection;

	int textureIndex;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> textures;
	std::unordered_map<std::string, float> floats;
	std::unordered_map<std::string, D3DXCOLOR> colors;

public:
	GAMEOBJECT* gameObject;

	Material() { reflection = false; textureIndex = 0; }
	virtual ~Material() {}

	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	bool GetReflection() { return reflection; }
	int* GetIndex() { return &textureIndex; }
	float GetFloat(std::string Name) { return floats[Name]; }
	D3DXCOLOR GetColor(std::string Name) { return colors[Name]; }
	ID3D11ShaderResourceView* GetTexture(std::string Name) { return textures[Name]; }

	void SetIndex(int value) { textureIndex = value; }
	void SetFloat(std::string Name, float value) { floats[Name] = value; }
	void SetColor(std::string Name, D3DXCOLOR value) { colors[Name] = value; }
	void SetTexture(std::string Name,ID3D11ShaderResourceView* value) { textures[Name] = value; }
	void SetTexture(std::string Name, TextureReader::READ_TEXTURE index) 
	{ 
		textures[Name] = TextureReader::GetReadTexture(index); 
		textureIndex = index;
	}
};

class SpriteMaterial : public Material
{
public:

	void Start() override;
	
	void Update() override {}
	void Draw() override;
};
class DefaultMaterial : public Material
{
public:

	void Start() override;
	
	void Update() override {}
	void Draw() override;
};
class UnlitMaterial : public Material
{
public:

	void Start() override;
	
	void Update() override {}
	void Draw() override;
};
class DissolveMaterial : public Material
{
public:

	void Start() override;

	void Update() override;
	void Draw() override;
};
class FieldDefaultMaterial : public Material
{
public:

	void Start() override;
	
	void Update() override;
	void Draw() override;
};
class WaterMaterial : public Material
{
public:

	void Start() override;

	void Update() override;
	void Draw() override;
};
class WipeMaterial : public Material
{
public:

	void Start() override;
	
	void Update() override {}
	void Draw() override;
};
class LitTextureMaterial : public Material
{
public:

	void Start() override;

	void Update() override {}
	void Draw() override;
};
class ToonMaterial : public Material
{
public:

	void Start() override;

	void Update() override;
	void Draw() override;
};
class MetallicMaterial : public Material
{
public:

	void Start() override;

	void Update() override;
	void Draw() override;
};
class GaugeMaterial : public Material
{
public:

	void Start() override;

	void Update() override {}
	void Draw() override;
};
class PostProcessMaterial : public Material
{
public:

	void Start() override;

	void Update() override {}
	void Draw() override;
};