#pragma once
#include <unordered_map>

#include "gameobject.h"
#include "textureReader.h"
#include "functions.h"

class Material
{
protected:
	std::unordered_map<std::string, ID3D11ShaderResourceView*> textures;
	std::unordered_map<std::string, float> floats;

public:
	GAMEOBJECT* gameObject;

	virtual ~Material() {}

	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	float GetFloat(std::string Name) { return floats[Name]; }
	ID3D11ShaderResourceView* GetTexture(std::string Name) { return textures[Name]; }

	void SetFloat(std::string Name, float value) { floats[Name] = value; }
	void SetTexture(std::string Name, ID3D11ShaderResourceView* tex) { textures[Name] = tex; }
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