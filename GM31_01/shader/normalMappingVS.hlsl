#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	float4 worldNormal, normal;
	//Normal
	normal = float4(In.Normal.xyz, 0.0f);
	worldNormal = mul(normal, World);
	Out.Normal = normalize(worldNormal);

	//Tangent
	normal = float4(In.Tangent.xyz, 0.0f);
	worldNormal = mul(normal, World);
	Out.Tangent = normalize(worldNormal);

	//Binormal
	normal = float4(In.Binormal.xyz, 0.0f);
	worldNormal = mul(normal, World);
	Out.Binormal = normalize(worldNormal);

	Out.Position = mul(In.Position, wvp);
	Out.WorldPosition = mul(In.Position, World);
	Out.Diffuse = In.Diffuse;
	Out.TexCoord = In.TexCoord;
}
