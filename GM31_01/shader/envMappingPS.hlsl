
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureEnv : register(t1);
TextureCube g_TextureEnvCube : register(t2);	//Cube Map
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//法線を正規化して格納する
	float4 normal = normalize(In.Normal);

	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev);

	float light = -dot(Light.Direction.xyz, normal.xyz);
	light = saturate(light);

	float d = dot(eyev, normal.xyz);
	if (d > -0.3f) { outDiffuse.rgb *= 0.0f; }

	//反射ベクトル
	float3 refv = reflect(eyev, normal.xyz);
	refv = normalize(refv);

	//Texture Refraction Mapping
	{
		//ベクトル値をテクスチャ座標に変換
		//float2 envTexCoord;
		//envTexCoord.x = -refv.x * 0.5f + 0.5f;
		//envTexCoord.y = -refv.y * 0.5f + 0.5f;

		//float4 EnvTex = g_TextureEnv.SampleBias(g_SamplerState, envTexCoord, Parameter.r);
		//float4 BaseTex = g_Texture.Sample(g_SamplerState, In.TexCoord);

		//outDiffuse.rgb = (EnvTex.rgb * 0.5f + BaseTex.rgb * 0.5f * light);
		//outDiffuse.a = In.Diffuse.a;
	}

	//Cube Refraction Mapping
	{
		float4 CubeEnvTex = g_TextureEnvCube.Sample(g_SamplerState, refv);
		float4 BaseTex = g_Texture.Sample(g_SamplerState, In.TexCoord);

		outDiffuse.rgb = (CubeEnvTex.rgb * 1.0f + BaseTex.rgb * 0.001f * light);
		outDiffuse.a = In.Diffuse.a;
	}
}
