#pragma once
#include <unordered_map>

#include "../MANAGEMENT FUNCTIONS/gameobject.h"
#include "../PRIVATE PATTERN/textureReader.h"

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

	void Start() override
	{
		gameObject->SetDepthShadow(false);

		SetTexture("_Texture", nullptr);
		SetTexture("_Normal_Map", nullptr);

		Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\unlitTextureVS.cso");
		Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\unlitTexturePS.cso");
	}
	
	void Update() override {}
	void Draw() override
	{
		if (textures["_Texture"] != nullptr)	{ Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }
		if (textures["_Normal_Map"] != nullptr)	{ Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &textures["_Normal_Map"]); }

	}
};
class Default : public Material
{
public:

	void Start() override 
	{
		gameObject->SetDepthShadow(true);

		SetTexture("_Texture", /*TextureReader::GetReadTexture(TextureReader::BOX_T)*/nullptr);
		SetTexture("_Normal_Map", nullptr);

		Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\vertexLightingVS.cso");
		Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\vertexLightingPS.cso");
	}
	
	void Update() override {}
	void Draw() override
	{
		if (textures["_Texture"] != nullptr)	{ Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }
		if (textures["_Normal_Map"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &textures["_Normal_Map"]); }

	}
};
class Unlit : public Material
{
public:

	void Start() override
	{
		gameObject->SetDepthShadow(false);

		SetTexture("_Texture", nullptr);
		SetTexture("_Normal_Map", nullptr);

		Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\unlitDefaultVS.cso");
		Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\unlitDefaultPS.cso");
	}
	
	void Update() override {}
	void Draw() override
	{
		if (textures["_Texture"] != nullptr)	{ Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }
		if (textures["_Normal_Map"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &textures["_Normal_Map"]); }

		PARAMETER param;
		ZeroMemory(&param, sizeof(param));
		param.dissolveThreshold = 0.0f;
		param.dissolveRange = 0.0f;
		param.color = gameObject->GetColor();

		Renderer::SetParameter(param);
	}
};
class Dissolve : public Material
{
public:

	void Start() override
	{
		gameObject->SetDepthShadow(true);

		SetFloat("_Threshold", 1.0f);
		SetFloat("_Delta", 0.0075f);
		SetFloat("_Dissolve_Range", 0.3f);

		SetTexture("_Texture", TextureReader::GetReadTexture(TextureReader::BOX_T));
		SetTexture("_Dissolve_Texture", TextureReader::GetReadTexture(TextureReader::DISSOLVE_T));

		Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\DepthShadowMappingVS.cso");
		Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\DissolveDepthShadowPS.cso");

	}

	void Update() override
	{
		if (floats["_Threshold"] > 1.1f || floats["_Threshold"] < 0.0f) { floats["_Delta"] *= -1.0f; }
		floats["_Threshold"] += floats["_Delta"];
	}

	void Draw() override
	{
		ID3D11ShaderResourceView* depthShadowTexture;
		depthShadowTexture = Renderer::GetDepthShadowTexture();

		if (textures["_Texture"] != nullptr)			{ Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }
		if (textures["_Dissolve_Texture"] != nullptr)	{ Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &textures["_Dissolve_Texture"]); }
		if (depthShadowTexture != nullptr)				{ Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, &depthShadowTexture); }

		PARAMETER param;
		ZeroMemory(&param, sizeof(param));
		param.dissolveThreshold = (1.0f - floats["_Threshold"]);
		param.dissolveRange = floats["_Dissolve_Range"];
		param.color = gameObject->GetColor();

		Renderer::SetParameter(param);
	}
};
class FieldDefault : public Material
{
public:

	void Start() override
	{
		gameObject->SetDepthShadow(true);
		
		SetFloat("_Time", 0.0f);
		SetFloat("_Frequency", 0.0f);

		SetTexture("_Texture", TextureReader::GetReadTexture(TextureReader::BOX_T));

		Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\DepthShadowMappingVS.cso");
		Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\unlitWaterPS.cso");
	}
	
	void Update() override 
	{
		floats["_Time"] += 1.0f / FRAME_RATE;

		floats["_Frequency"] = sinf(floats["_Time"]);

	}
	void Draw() override
	{
		ID3D11ShaderResourceView* depthShadowTexture;
		depthShadowTexture = Renderer::GetDepthShadowTexture();

		if (textures["_Texture"] != nullptr)	{ Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }
		//if (depthShadowTexture != nullptr)		{ Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &depthShadowTexture); }

		PARAMETER param;
		ZeroMemory(&param, sizeof(param));
		param.dissolveThreshold = floats["_Frequency"];
		param.dissolveRange = floats["_Time"];
		param.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		Renderer::SetParameter(param);
	}
};
class Wipe : public Material
{
public:

	void Start() override
	{
		gameObject->SetDepthShadow(false);

		SetFloat("_Threshold", 1.0f);
		SetFloat("_Delta", 0.03f);

		SetTexture("_Texture", TextureReader::GetReadTexture(TextureReader::BOX_T));
		SetTexture("_Wipe_Texture", TextureReader::GetReadTexture(TextureReader::WIPE_T));

		Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\unlitTextureVS.cso");
		Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\wipePS.cso");
	}
	
	void Update() override {}
	void Draw() override
	{
		if (textures["_Texture"] != nullptr)		{ Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }
		if (textures["_Wipe_Texture"] != nullptr)	{ Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &textures["_Wipe_Texture"]); }

		PARAMETER param;
		ZeroMemory(&param, sizeof(param));
		param.dissolveThreshold = (1.0f - floats["_Threshold"]);
		param.dissolveRange = 0.3f;
		param.color = gameObject->GetColor();

		Renderer::SetParameter(param);
	}
};
class LitTexture : public Material
{
public:

	void Start() override
	{
		gameObject->SetDepthShadow(false);

		SetTexture("_Texture", nullptr);
		SetTexture("_Normal_Map", nullptr);

		Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\unlitDefaultVS.cso");
		Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\litTextureColoringPS.cso");
	}

	void Update() override {}
	void Draw() override
	{
		if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }
		if (textures["_Normal_Map"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &textures["_Normal_Map"]); }

		PARAMETER param;
		ZeroMemory(&param, sizeof(param));
		param.dissolveThreshold = 0.0f;
		param.dissolveRange = 0.0f;
		param.color = gameObject->GetColor();

		Renderer::SetParameter(param);
	}
};