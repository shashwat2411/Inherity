
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	Out.Position = mul(In.Position, wvp);

	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0f);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	Out.Normal = worldNormal;

	//��������
	float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	light = saturate(light);

	Out.Diffuse = In.Diffuse * Material.Diffuse * light * Light.Diffuse;
	Out.Diffuse += In.Diffuse * Material.Ambient * Light.Ambient;
	Out.Diffuse += Material.Emission;
	Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;

	Out.Tangent = float4(0.0f, 0.0f, 0.0f, 0.0f);
	Out.Binormal = float4(0.0f, 0.0f, 0.0f, 0.0f);
	Out.TexCoord = In.TexCoord;

	matrix lightwvp;
	lightwvp = mul(World, Light.View);
	lightwvp = mul(lightwvp, Light.Projection);

	Out.ShadowPosition = float4(0.0f, 0.0f, 0.0f, 0.0f);
	Out.WorldPosition = float4(0.0f, 0.0f, 0.0f, 0.0f);

}

