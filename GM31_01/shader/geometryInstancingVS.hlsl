
#include "common.hlsl"

//float4x4 MatrixMul(float4 value)
//{
//	matrix identity = float4x4(
//		value.x, 0.0f, 0.0f, 0.0f,
//		0.0f, value.y, 0.0f, 0.0f,
//		0.0f, 0.0f, value.z, 0.0f,
//		0.0f, 0.0f, 0.0f, value.w
//		);
//
//	return identity;
//}

float4x4 MatrixScaling(float4 value) {

	matrix identity = float4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	identity[0][0] = value.x;
	identity[1][1] = value.y;
	identity[2][2] = value.z;

	return identity;
}

float4x4 MatrixRotationYawPitchRoll(float4 value) {

	matrix identity = float4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	// Apply rotations
	float cy = cos(value.y);
	float sy = sin(value.y);
	float cp = cos(value.x);
	float sp = sin(value.x);
	float cr = cos(value.z);
	float sr = sin(value.z);

	identity[0][0] = cy * cr + sy * sp * sr;
	identity[0][1] = sy * cp;
	identity[0][2] = -cy * sr + sy * sp * cr;

	identity[1][0] = -sy * cr + cy * sp * sr;
	identity[1][1] = cy * cp;
	identity[1][2] = sy * sr + cy * sp * cr;

	identity[2][0] = cp * sr;
	identity[2][1] = 0-sp;
	identity[2][2] = cp * cr;

	return identity;
}

float4x4 MatrixTranslation(float4 value) {

	matrix identity = float4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	identity[3][0] = value.x;
	identity[3][1] = value.y;
	identity[3][2] = value.z;

	return identity;
}

void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	{
		matrix transform, scale, rotation, position;

		scale = MatrixScaling(float4(Scale[In.InstanceID], 1.0f));
		rotation = MatrixRotationYawPitchRoll(float4(Rotation[In.InstanceID], 1.0f));
		position = MatrixTranslation(float4(Position[In.InstanceID], 1.0f));

		transform = mul(scale, rotation);
		transform = mul(transform, position);
		transform = mul(transform, wvp);

		In.Position = mul(In.Position, transform);
		Out.Position = In.Position;
	}

	//In.Position.xyz += Position[In.InstanceID];
	//Out.Position = mul(In.Position, wvp);

	Out.TexCoord = In.TexCoord;

	Out.Diffuse = In.Diffuse;


	Out.WorldPosition = float4(0.0, 0.0, 0.0, 0.0);
	Out.ShadowPosition = float4(0.0, 0.0, 0.0, 0.0);
	Out.Normal = In.Normal;
	Out.Tangent = float4(0.0, 0.0, 0.0, 0.0);
	Out.Binormal = float4(0.0, 0.0, 0.0, 0.0);
}