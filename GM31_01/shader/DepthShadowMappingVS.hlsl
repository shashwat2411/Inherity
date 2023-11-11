
#include "common.hlsl"



void main(in VS_IN In, out PS_IN Out)
{
	float4 pos = In.Position;

	float frequency = dissolveThreshold;
	float speed = 1.0f * dissolveRange;
	float value = sqrt(pos.x*pos.x + pos.z*pos.z);

	float final_y = frequency * value + speed;
	final_y = sin(final_y) + pos.z;

	float4 position = float4(pos.x, pos.y + final_y, pos.z, pos.w);

	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	Out.Position = mul(position, wvp);

	//matrix wvp;
	//wvp = mul(World, View);
	//wvp = mul(wvp, Projection);
	//Out.Position = mul(In.Position, wvp);

	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0f);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	Out.Normal = worldNormal;

	//åıåπèàóù
	float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	light = saturate(light);
	Out.Diffuse = light;
	Out.Diffuse.a = In.Diffuse.a;

	Out.Tangent = float4(0.0f, 0.0f, 0.0f, 0.0f);
	Out.Binormal = float4(0.0f, 0.0f, 0.0f, 0.0f);
	Out.TexCoord = In.TexCoord;

	matrix lightwvp;
	lightwvp = mul(World, Light.View);
	lightwvp = mul(lightwvp, Light.Projection);

	Out.ShadowPosition = mul(In.Position, lightwvp);
	Out.WorldPosition = float4(0.0f, 0.0f, 0.0f, 0.0f);

}
