
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 Tex = g_Texture.Sample(g_SamplerState, In.TexCoord);
	float noise = clamp(0.2f, 0.5f, Rand(In.TexCoord * 0.1f));

	Tex.rgb += noise;

	float gray;
	gray = 0.299f*Tex.r + 0.587f*Tex.g + 0.114f*Tex.b;

	Tex.r = gray;
	Tex.g = gray;
	Tex.b = gray;

	outDiffuse.rgb = saturate(Tex.rgb) * In.Diffuse;
	outDiffuse.a = Tex.a * In.Diffuse.a;
}
