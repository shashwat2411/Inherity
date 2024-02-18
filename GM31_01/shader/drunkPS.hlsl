
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s1);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 original = g_Texture.Sample(g_SamplerState, In.TexCoord);

	float x = In.TexCoord.x;
	float y = In.TexCoord.y;

	float result = sin(x * 8.0f + dissolveRange) * 0.03f;


	float2 texCoord = float2(x, y + result);

	float4 distorted = g_Texture.Sample(g_SamplerState, texCoord);

	float4 finalColor = distorted * 1.2f + original * 0.8f;

	outDiffuse = finalColor;
	outDiffuse *= In.Diffuse;


	outDiffuse.a = 1.0f;
}
