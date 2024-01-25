

#include "common.hlsl"


Texture2D g_Texture : register(t0);
Texture2D g_NormalMap : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	if (Material.TextureEnable)
	{
		outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
		outDiffuse *= In.Diffuse * color;

	}
	else
	{
		outDiffuse = In.Diffuse * color;
	}

	float4 normal = normalize(In.Normal);
	if (dissolveRange > 1.0f)
	{
		float4 normalMap = g_NormalMap.Sample(g_SamplerState, In.TexCoord);
		normalMap = (normalMap * 2.0f) - 1.0f;

		float4 n = normalize(In.Normal);
		float4 t = normalize(In.Tangent);
		float4 b = normalize(In.Binormal);

		normal.xyz = t.xyz * normalMap.x + b.xyz * normalMap.y + n.xyz * normalMap.z;
		normal.w = 0.0;
	}


	if (outDiffuse.a > 0.01f)
	{
		//ñ@ê¸Çê≥ãKâªÇµÇƒäiî[Ç∑ÇÈ
		float light = -dot(Light.Direction.xyz, normal.xyz);
		light = saturate(light);

		outDiffuse.rgb *= light + 0.3f;
		outDiffuse.a *= In.Diffuse.a;

		float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
		eyev = normalize(eyev);
		float3 halfv = eyev + Light.Direction.xyz;
		halfv = normalize(halfv);

		float specular = -dot(halfv, normal.xyz);
		saturate(specular);
		specular = pow(abs(specular), 60);
		outDiffuse.rgb = saturate(outDiffuse.rgb + specular);
	}
	else { discard; }


}

