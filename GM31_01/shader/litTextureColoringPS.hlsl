#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);

	if (outDiffuse.a > 0.01f)
	{
		float3 monoChrome;

		monoChrome.r = 0.21f * outDiffuse.r;
		monoChrome.g = 0.72f * outDiffuse.g;
		monoChrome.b = 0.07f * outDiffuse.b;

		float4 coloring;
		coloring.rgb = monoChrome.r + monoChrome.g + monoChrome.b;
		coloring = round(coloring);
		coloring.a = 1.0f;

		float4 color1 = color;
		float4 color2 = float4(0.0f, 0.0f, 0.0f, 0.0f);

		color1 *= coloring;
		color2 *= (1.0f - coloring);
		color2.a = 0.0f;

		float4 finalColor = color1 + color2;

		outDiffuse = finalColor;

		if (outDiffuse.r < 0.01f && outDiffuse.g < 0.01f && outDiffuse.b < 0.01f) { discard; }
	}
	else { discard; }


}

