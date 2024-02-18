
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_WipeTexture : register(t1);
SamplerState g_MirrorSamplerState : register(s2);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float offset = dissolveThreshold;

	outDiffuse = g_Texture.Sample(g_MirrorSamplerState, In.TexCoord - float2(0.0f, -offset - 0.34f));
	outDiffuse *= In.Diffuse;
}
