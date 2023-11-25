
#include "common.hlsl"



void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	Out.Position = mul(In.Position, wvp);

	Out.TexCoord = In.TexCoord;

	Out.Diffuse = In.Diffuse;

	Out.WorldPosition = float4(0.0, 0.0, 0.0, 0.0);
	Out.Normal = In.Normal;
	Out.Tangent = float4(0.0, 0.0, 0.0, 0.0);
	Out.Binormal = float4(0.0, 0.0, 0.0, 0.0);
}
