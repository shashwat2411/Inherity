
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{
	//Position
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	Out.Position = mul(In.Position, wvp);

	//Normal
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0f);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	Out.Normal = worldNormal;

	//Diffuse
	Out.Diffuse = In.Diffuse * Material.Diffuse * Light.Diffuse;
	//Out.Diffuse += In.Diffuse * Material.Ambient * Light.Ambient;
	Out.Diffuse += Material.Emission;
	Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;

	//Tangent
	Out.Tangent = float4(0.0f, 0.0f, 0.0f, 0.0f);
	if (dissolveRange > 1.0f)
	{
		normal = float4(In.Tangent.xyz, 0.0f);
		worldNormal = mul(normal, World);
		Out.Tangent = normalize(worldNormal);
	}

	//Binormal
	Out.Binormal = float4(0.0f, 0.0f, 0.0f, 0.0f);
	if (dissolveRange > 1.0f)
	{
		normal = float4(In.Binormal.xyz, 0.0f);
		worldNormal = mul(normal, World);
		Out.Binormal = normalize(worldNormal);
	}
	
	Out.TexCoord = In.TexCoord;

	matrix lightwvp;
	lightwvp = mul(World, Light.View);
	lightwvp = mul(lightwvp, Light.Projection);

	Out.ShadowPosition = float4(0.0f, 0.0f, 0.0f, 0.0f);
	Out.WorldPosition = mul(In.Position, World);

}

