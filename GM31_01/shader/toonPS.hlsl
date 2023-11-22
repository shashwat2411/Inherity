
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureToon : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//法線を正規化して格納する
	float4 normal = normalize(In.Normal);

	//テクスチャから色をサンプリングする
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.a *= In.Diffuse.a;

	//ランバート
	float light = -dot(normal.xyz, Light.Direction.xyz);
	//float brightness = clamp(light, 0.01f, 0.99f);

	float2 coord = float2(light, 0.5f);
	float4 lightColor = g_TextureToon.Sample(g_SamplerState, coord);

	outDiffuse.rgb *= lightColor.rgb;

	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev);

	float d = dot(eyev, normal.xyz);
	if (d > -0.3f) { outDiffuse.rgb *= 0.0f; }

	float pattern = 0.03125f + 0.0625f * 15.0f;
	float2 uv = light;
	float3 col;

	uv.x += dissolveRange;
	uv.y = pattern;

	col = g_TextureToon.Sample(g_SamplerState, uv);
	outDiffuse.rgb *= saturate(In.Diffuse.rgb * col);
}
