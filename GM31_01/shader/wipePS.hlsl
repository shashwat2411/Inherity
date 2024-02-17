
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_WipeTexture : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//float2 coord = float2(In.TexCoord.x, max(In.TexCoord.y, 0.5f));

	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;
	//outDiffuse.a = 1.0f;

	//r ‚ÌF‚¾‚¯‚ğ‹‚ß‚é
	float dissolveValue = g_WipeTexture.Sample(g_SamplerState, In.TexCoord);
	float threshold = dissolveThreshold * (1.0f + dissolveRange) - dissolveRange;
	float rate = saturate((dissolveValue - threshold) / dissolveRange);

	outDiffuse.a = rate;



}
