cbuffer WorldBuffer : register(b0)
{
	matrix World;
}

cbuffer ViewBuffer : register(b1)
{
	matrix View;
}

cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}

struct VS_IN
{
	float4 Position		: POSITION0;
	float4 Normal		: NORMAL0;
	float4 Tangent		: TANGENT0;
	float4 Binormal		: BINORMAL0;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;
};

struct PS_IN
{
	float4 Position			: SV_POSITION;
	float4 WorldPosition	: POSITION0;
	float4 Normal			: NORMAL0;
	float4 Tangent			: TANGENT0;
	float4 Binormal			: BINORMAL0;
	float4 Diffuse			: COLOR0;
	float2 TexCoord			: TEXCOORD0;

	float4 ShadowPosition	: POSITION1;	//ライトカメラから見たピクセルの座標
};

struct MATERIAL
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float Shininess;
	bool TextureEnable;
	float2 Dummy;
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL Material;
}


struct LIGHT
{
	bool Enable;
	bool3 Dummy;
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;

	matrix View;
	matrix Projection;
};

cbuffer LightBuffer : register(b4)
{
	LIGHT Light;
}

cbuffer CameraBuffer : register(b5)
{
	float4 CameraPosition;
}


cbuffer ParamBuffer : register(b6)
{
	float dissolveThreshold;
	float dissolveRange;
	float2 dummy;
	float4 color;
	float4 color2;
	float4 color3;
	float4 color4;
}

float Rand(float2 coord)
{
	//frac(x) = x の少数部を返す　乱数は 0.0 <= rand < 1.0 の範囲
	return frac(sin(dot(coord.xy, float2(12.9898f, 78.233f))) * 43758.5453f);
}