
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;

	//グレースケール変換
	float gray;
	gray = 0.299 * outDiffuse.r + 0.587 * outDiffuse.g + 0.114 * outDiffuse.b;
	
	clip(gray - 1.0f);
}
