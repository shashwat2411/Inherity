
#include "common.hlsl"



void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;//matrixå^ÇÃïœêîwvpÇíËã`Ç∑ÇÈ

	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	Out.Position = mul(In.Position, wvp);
	Out.TexCoord = In.TexCoord;

	float4 worldNormal, normal;

	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	Out.Normal = worldNormal;
	Out.Tangent = float4(0.0, 0.0, 0.0, 0.0);
	Out.Binormal = float4(0.0, 0.0, 0.0, 0.0);

	Out.Diffuse = In.Diffuse * Material.Diffuse * Light.Diffuse;
	//Out.Diffuse += In.Diffuse * Material.Ambient * Light.Ambient;
	Out.Diffuse += Material.Emission;
	Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;

	Out.WorldPosition = mul(In.Position, World);
	Out.ShadowPosition = float4(0.0f, 0.0f, 0.0f, 0.0f);
}
