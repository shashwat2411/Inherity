
#include "common.hlsl"

#define FXAA_PC 1
#define FXAA_HLSL_5 1
#define FXAA_QUALITY__PRESET 12
#include "FXAA.hlsl"	

void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	Out.Position = mul(In.Position, wvp);

	Out.TexCoord = In.TexCoord;

	Out.Diffuse = In.Diffuse;

	Out.WorldPosition = float4(0.0, 0.0, 0.0, 0.0);
	Out.ShadowPosition = float4(0.0, 0.0, 0.0, 0.0);
	Out.Normal = In.Normal;
    Out.Tangent = float4(0.0, 0.0, 0.0, 0.0);
    Out.Binormal = float4(0.0, 0.0, 0.0, 0.0);
}
