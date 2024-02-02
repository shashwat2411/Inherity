
#include "common.hlsl"

float4x4 MatrixMul(float4 value)
{
	matrix identity = float4x4(
		value.x, 0.0f, 0.0f, 0.0f,
		0.0f, value.y, 0.0f, 0.0f,
		0.0f, 0.0f, value.z, 0.0f,
		0.0f, 0.0f, 0.0f, value.w
		);

	return identity;
}

void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	{
		//matrix transform, scale, rotation, position;

		//scale = MatrixMul(float4(Scale[In.InstanceID], 1.0f));
		//rotation = MatrixMul(float4(Rotation[In.InstanceID], 1.0f));
		//position = MatrixMul(float4(Position[In.InstanceID], 1.0f));

		//transform = mul(scale, rotation);
		//transform = mul(transform, position);
		//transform = mul(transform, wvp);

		//In.Position += mul(In.Position, transform);
		//Out.Position = In.Position;
	}

	In.Position.xyz += Position[In.InstanceID];
	Out.Position = mul(In.Position, wvp);

	Out.TexCoord = In.TexCoord;

	Out.Diffuse = In.Diffuse;


	Out.WorldPosition = float4(0.0, 0.0, 0.0, 0.0);
	Out.ShadowPosition = float4(0.0, 0.0, 0.0, 0.0);
	Out.Normal = In.Normal;
	Out.Tangent = float4(0.0, 0.0, 0.0, 0.0);
	Out.Binormal = float4(0.0, 0.0, 0.0, 0.0);
}