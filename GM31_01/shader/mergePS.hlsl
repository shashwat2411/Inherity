
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_BloomTexture01 : register(t1);
Texture2D g_BloomTexture02 : register(t2);
Texture2D g_BloomTexture03 : register(t3);
Texture2D g_BloomTexture04 : register(t4);
Texture2D g_BloomTexture05 : register(t5);
Texture2D g_BloomTexture06 : register(t6);
Texture2D g_BloomTexture07 : register(t7);
SamplerState g_SamplerState : register(s1);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;

	float4 bloom = 0.0f;

	bloom += g_BloomTexture01.Sample(g_SamplerState, In.TexCoord) * 0.6;
	bloom += g_BloomTexture02.Sample(g_SamplerState, In.TexCoord) * 0.5;
	bloom += g_BloomTexture03.Sample(g_SamplerState, In.TexCoord) * 0.4;
	bloom += g_BloomTexture04.Sample(g_SamplerState, In.TexCoord) * 0.2;
	bloom += g_BloomTexture05.Sample(g_SamplerState, In.TexCoord) * 0.05f;
	bloom += g_BloomTexture06.Sample(g_SamplerState, In.TexCoord) * 0.5f;
	bloom += g_BloomTexture07.Sample(g_SamplerState, In.TexCoord) * 0.5f;

	outDiffuse.rgb += bloom.rgb * Light.Diffuse.rgb;

	outDiffuse.a = 1.0f;
}