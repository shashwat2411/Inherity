

#include "common.hlsl"


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

	if (Material.TextureEnable)
	{
		outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
		outDiffuse *= In.Diffuse;

	}
	else
	{
		outDiffuse = In.Diffuse;
	}

	if (outDiffuse.a > 0.01f)
	{
		//ñ@ê¸Çê≥ãKâªÇµÇƒäiî[Ç∑ÇÈ
		float4 normal = normalize(In.Normal);
		float light = -dot(Light.Direction.xyz, normal.xyz);
		light = saturate(light);

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
	else { discard; }


}

