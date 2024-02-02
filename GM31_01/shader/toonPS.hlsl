
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureToon : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 normal = normalize(In.Normal);

	outDiffuse = In.Diffuse;
	
	float light = -dot(normal.xyz, Light.Direction.xyz);

	if (light > 0.7f) { light = 1.0f; }
	else if (light > 0.4f) { light = 0.7f; }
	else { light = 0.5f; }

	outDiffuse.rgb *= saturate(light);

	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev);

	float d = dot(eyev, normal.xyz);

	if (d > -0.4f) { outDiffuse.rgb *= 0.0; }
}
