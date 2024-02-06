
#include "common.hlsl"


Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;

	float3 coord;
	coord = color * 0.5f + 0.5f;

	if (abs(In.TexCoord.x - coord.x) < 0.01f && abs(In.TexCoord.y - coord.z) < 0.01f)
	{
		outDiffuse.rgb = float3(1.0f, 0.0f, 0.0f);
	}

	if (outDiffuse.a <= 0.01f) { discard; }

}
