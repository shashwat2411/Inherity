#include "common.hlsl"

Texture2D		g_Texture : register(t0);
Texture2D		g_TextureDepthShadow1 : register(t1);
SamplerState	g_SamplerState : register(s0);
SamplerState	g_SamplerStateClamp : register(s1);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	const int pcfCount = 1;
	const float totalTexels = (pcfCount * 2.0f + 1.0f) * (pcfCount * 2.0f + 1.0f);
	const float mapSize = 4096;
	float texelSize = 1.0f / mapSize;
	float total = 0.0f;
	//日向での色　＝　普通のテクスチャ　＊　頂点色を作成しておく
	
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;

	In.ShadowPosition.xyz /= In.ShadowPosition.w;
	In.ShadowPosition.x = In.ShadowPosition.x * 0.5f + 0.5f;
	In.ShadowPosition.y = -In.ShadowPosition.y * 0.5f + 0.5f;


	for (int x = -pcfCount; x <= pcfCount; x++)
	{
		for (int y = -pcfCount; y <= pcfCount; y++)
		{
			float depth2 = g_TextureDepthShadow1.Sample(g_SamplerStateClamp, In.ShadowPosition.xy + float2(x, y) * texelSize);

			if (depth2 < In.ShadowPosition.z - 0.005f)
			{
				total += 1.0f;
			}
		}
	}

	float multiplier = 1.0f;
	if ((In.ShadowPosition.x > 4.0f || In.ShadowPosition.x < -1.0f) || (In.ShadowPosition.y > 4.0f || In.ShadowPosition.y < -1.0f))
	{
		if (total >= totalTexels)
		{
			multiplier = 0.0f;
		}
	}

	total /= totalTexels;
	float lightFactor = 1.0f - total * multiplier;

	outDiffuse.rgb *= max(In.Diffuse.rgb * lightFactor, 0.3f);
	outDiffuse.a *= In.Diffuse.a;

	outDiffuse *= color;


	//outDiffuse.rgb *= 0.5f;

}
