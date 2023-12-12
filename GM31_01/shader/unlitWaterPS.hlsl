

#include "common.hlsl"


Texture2D g_Texture : register(t0);
Texture2D g_TextureMirror : register(t1);
SamplerState g_SamplerState : register(s0);
SamplerState g_SamplerStateClamp : register(s1);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	//outDiffuse *= In.Diffuse;

	float4 mirror = g_TextureMirror.Sample(g_SamplerStateClamp , In.TexCoord);
	mirror.a = 1.0f;

	outDiffuse = mirror;
	outDiffuse *= color2;

	//outDiffuse.a = 1.0f;
}

