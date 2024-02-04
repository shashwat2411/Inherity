
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	/*
	color.x = Current HP
	color.y = Max HP
	color.z = Difference HP
	*/

	outDiffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);

	if (In.TexCoord.x < dummy.x || In.TexCoord.x > (1.0f - dummy.x) || In.TexCoord.y < dummy.y || In.TexCoord.y > (1.0f - dummy.y))
	{
		return;
	}

	float4 text = g_Texture.Sample(g_SamplerState, In.TexCoord);

	//Square Gauge
	float gauge = In.TexCoord * color.y;
	outDiffuse.rgb += text * color3.rgb * step(color.z, gauge);
	outDiffuse.rgb += text * color2.rgb * (1.0f - step(color.x, gauge));
	outDiffuse.rgb += text * color4.rgb * (1.0f - step(color.z, gauge)) * step(color.x, gauge);

}

int step(float a, float b)
{
	if (b >= a) { return 1; }
	return 0;
}