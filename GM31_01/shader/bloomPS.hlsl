
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s1);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;
	
	float4 bloom = 0.0f;

	//画面サイズ X960 Y540とした場合
	for (int y = (int)color.x; y <= (int)color.y; y++)	//上下は±12ピクセルの範囲
	{
		for (int x = (int)color.z; x <= (int)color.w; x++)	//左右は±3ピクセルの範囲
		{
			//式中の値の3を調節すると光の範囲が変化する
			float2 texCoord = In.TexCoord + float2(x * color2.x / (960.0f*dummy.x), y * color2.y / (540.0f*dummy.x)); //テクスチャ座標をずらす
			float4 color = g_Texture.Sample(g_SamplerState, texCoord);

			//明度を計算
			float bright = color.r * 0.299f + color.g * 0.587f + color.b * 0.114f;

			//明度が閾値0.7より大きい、かつ処理対象のピクセルでない
			if (bright > color2.z && (x != 0 || y != 0))
			{
				bloom += color / (x * x + y * y) * color2.w;
			}
		}
	}

	outDiffuse.rgb += bloom.rgb;
	outDiffuse.a = 1.0f;
	//outDiffuse.rgb *= Light.Diffuse.rgb;
}