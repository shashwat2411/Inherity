
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureToon : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	outDiffuse = In.Diffuse;
	//if (In.Diffuse.g >= 1.0f && In.Diffuse.r <= 0.0f && In.Diffuse.b <= 0.0f)
	//{
	//	outDiffuse.rgb = float3(1.0f, 0.0f, 0.0f);
	//}

	float4 normal = normalize(In.Normal);
	float light = -dot(normal.xyz, Light.Direction.xyz);
	float2 coord = float2(light, 0.5f);
	float4 lightColor = g_TextureToon.Sample(g_SamplerState, coord);
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev);

	//outline
	float d = dot(eyev, normal.xyz);
	if (d > -0.3f) { outDiffuse.rgb *= 0.0f; }

	float pattern = 0.03125f + 0.0625f * 15.0f;
	float2 uv = light;
	float4 col;

	uv.x += dissolveRange;
	uv.y = pattern;

}
