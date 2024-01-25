
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureToon : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	outDiffuse = In.Diffuse;

	float4 normal = normalize(In.Normal);
	float light = -dot(normal.xyz, Light.Direction.xyz);
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev);
	float2 coord = float2(light, 0.5f);
	float4 lightColor = g_TextureToon.Sample(g_SamplerState, coord);

	//outline
	float d = dot(eyev, normal.xyz);
	if (d > -0.3f) { outDiffuse.rgb *= 0.0f; }

	float pattern = 0.03125f + 0.0625f * 15.0f;
	float2 uv = light;
	float4 col;

	uv.x += dissolveRange;
	uv.y = pattern;

	//Phong
	light = -dot(Light.Direction.xyz, normal.xyz);
	light = saturate(light);

	outDiffuse.rgb *= light + 0.3f;
	outDiffuse.a *= In.Diffuse.a;

	float3 halfv = eyev + Light.Direction.xyz;
	halfv = normalize(halfv);

	float specular = -dot(halfv, normal.xyz);
	saturate(specular);
	specular = pow(abs(specular), 60);
	outDiffuse.rgb = saturate(outDiffuse.rgb + specular);
}
