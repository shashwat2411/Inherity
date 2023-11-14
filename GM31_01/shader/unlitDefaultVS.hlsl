
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	Out.Position = mul(In.Position, wvp);

	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0f);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	Out.Normal = worldNormal;

	Out.Diffuse.rgb = In.Diffuse.rgb * Material.Diffuse.rgb;
	Out.Diffuse.rgb += In.Diffuse.rgb * Material.Ambient.rgb;
	Out.Diffuse.rgb += Material.Emission.rgb;
	Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;

	Out.Tangent = float4(0.0f, 0.0f, 0.0f, 0.0f);
	Out.Binormal = float4(0.0f, 0.0f, 0.0f, 0.0f);
	Out.TexCoord = In.TexCoord;


	Out.ShadowPosition = float4(0.0, 0.0, 0.0, 0.0);
	Out.WorldPosition = float4(0.0f, 0.0f, 0.0f, 0.0f);

}

