
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_DissolveTexture : register(t1);
Texture2D g_TextureDepthShadow1 : register(t2);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	const int pcfCount = 2;
	const float totalTexels = (pcfCount * 2.0f + 1.0f) * (pcfCount * 2.0f + 1.0f);
	const float mapSize = 4096.0f;
	float texelSize = 1.0f / mapSize;
	float total = 0.0f;

	In.ShadowPosition.xyz /= In.ShadowPosition.w;
	In.ShadowPosition.x = In.ShadowPosition.x * 0.5f + 0.5f;
	In.ShadowPosition.y = -In.ShadowPosition.y * 0.5f + 0.5f;

	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;

	if (outDiffuse.a > 0.01f)
	{
		{
			//ñ@ê¸Çê≥ãKâªÇµÇƒäiî[Ç∑ÇÈ
			float4 normal = normalize(In.Normal);
			float light = -dot(Light.Direction.xyz, normal.xyz);
			light = saturate(light);

			outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
			outDiffuse.rgb *= light;
			outDiffuse.a *= In.Diffuse.a;

			float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
			eyev = normalize(eyev);
			float3 halfv = eyev + Light.Direction.xyz;
			halfv = normalize(halfv);

			float specular = -dot(halfv, normal.xyz);
			saturate(specular);
			specular = pow(specular, 30);
			outDiffuse.rgb = saturate(outDiffuse.rgb + specular);
		}

		for (int x = -pcfCount; x <= pcfCount; x++)
		{
			for (int y = -pcfCount; y <= pcfCount; y++)
			{
				float depth2 = g_TextureDepthShadow1.Sample(g_SamplerState, In.ShadowPosition.xy + float2(x, y) * texelSize);

				if (depth2 < In.ShadowPosition.z - 0.001f)
				{
					total += 1.0f;
				}
			}
		}
	}
	else { discard; }

	//r ÇÃêFÇæÇØÇãÅÇﬂÇÈ
	float dissolveValue = g_DissolveTexture.Sample(g_SamplerState, In.TexCoord);
	float threshold = dissolveThreshold * (1.0f + dissolveRange) - dissolveRange;
	float rate = saturate((dissolveValue - threshold) / dissolveRange);

	outDiffuse.a = rate;

	outDiffuse.rgb = lerp(outDiffuse.rgb, outDiffuse.rgb*float3(1.0f, 0.0f, 0.0f), 1.0f - pow(rate, 5));

	if (outDiffuse.a > 0.01f)
	{
		total /= totalTexels;
		float lightFactor = 1.0f - total;

		outDiffuse.rgb *= max(In.Diffuse.rgb * lightFactor, 0.1f);
		outDiffuse.a *= In.Diffuse.a;
	}
	else
	{
		discard;
	}

	outDiffuse *= color;

}