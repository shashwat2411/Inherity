
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float2 uv = In.TexCoord;
	float2 screen = float2(960, 540);

	float pixel = 0.1f;

	uv *= screen;	//UV値をスクリーン座標へ変換する
	uv /= pixel;	//矩形のサイズで割る
	uv = floor(uv);	//少数部を抹消
	uv *= pixel;		//スクリーン座標に戻す
	uv /= screen;	//スクリーン座標からUV値へ変換する

	outDiffuse = g_Texture.Sample(g_SamplerState, uv);
	outDiffuse *= In.Diffuse;
	outDiffuse.a = 1.0f;
}
