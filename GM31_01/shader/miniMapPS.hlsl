
#include "common.hlsl"

Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;

	float3 coord;
	coord = color * 0.5f + 0.5f;

	float2 center = float2(coord.x, coord.z);
	float radius = color2.y; // You can adjust the radius as needed

	// Calculate the distance from the current pixel to the center of the circle
	float distance = length(In.TexCoord.xy - center);

	float outline = color2.a * 1.5f;

	if (abs(In.TexCoord.x - coord.x) < outline && abs(In.TexCoord.y - coord.z) < outline)
	{
		if (distance <= outline)
		{
			outDiffuse.rgb = float3(0.0f, 0.0f, 0.0f);
		}
	}
	if (abs(In.TexCoord.x - coord.x) < color2.a && abs(In.TexCoord.y - coord.z) < color2.a)
	{
		if (distance <= color2.a)
		{
			outDiffuse.rgb = float3(1.0f, 0.0f, 0.0f);
			return;
		}
	}

	float clip = color2.x;
	if (In.TexCoord.x > (coord.x + clip) || In.TexCoord.x < (coord.x - clip) && (In.TexCoord.x <= 1.0f && In.TexCoord.x >= 0.0f))
	{
		discard;
	}
	if (In.TexCoord.y > (coord.z + clip) || In.TexCoord.y < (coord.z - clip) && (In.TexCoord.y <= 1.0f && In.TexCoord.y >= 0.0f))
	{
		discard;
	}

	// Check if the pixel is inside the circle
	if (distance > radius)
	{
		discard;
	}
	else if (distance >= (radius - color2.z))
	{
		outDiffuse.rgb = float3(0.0f, 0.0f, 0.0f);
	}


}
