#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureNormalMap : register(t1);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 normalMap = g_TextureNormalMap.Sample(g_SamplerState, In.TexCoord);
	normalMap = (normalMap * 2.0f) - 1.0f;

	float4 n = normalize(In.Normal);
	float4 t = normalize(In.Tangent);
	float4 b = normalize(In.Binormal);
	float4 normal;
	normal.xyz = t.xyz * normalMap.x + b.xyz * normalMap.y + n.xyz * normalMap.z;
	normal.w = 0.0f;
	normal = normalize(normal);


	//光源の向きと回転済みの法線とで内積を計算し、符号を反転して輝度を求める
	float light = -dot(Light.Direction.xyz, normal.xyz);
	//飽和演算(0.0～1.0の値にクランプする)
	light = saturate(light);

	//テクスチャから色をサンプリングする
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);

	//テクスチャの色に輝度を乗算する
	outDiffuse.rgb *= light;

	//透明度は輝度に影響しないのでそのまま使う
	outDiffuse.a *= In.Diffuse.a;

	//ブリンフォン鏡面反射
	//視線ベクトルを作成する（カメラからピクセルへ向かうベクトル）
	float3 evev = In.WorldPosition.xyz - CameraPosition.xyz;
	evev = normalize(evev);
	//ハーフベクトルを作成する（視線ベクトル＋ライトベクトル）
	float3 halfv = evev + Light.Direction.xyz;
	halfv = normalize(halfv);

	float specular = -dot(halfv, normal.xyz);
	saturate(specular);
	specular = pow(specular, 5);
	outDiffuse.rgb = saturate(outDiffuse.rgb + specular);

	outDiffuse *= color;
}