
#include "common.hlsl"

Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;

	float3 coord;
	coord = color * 0.5f + 0.5f;

	if (abs(In.TexCoord.x - coord.x) < 0.01f && abs(In.TexCoord.y - coord.z) < 0.01f)
	{
		outDiffuse.rgb = float3(1.0f, 0.0f, 0.0f);
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

	float2 center = float2(coord.x, coord.z);
	float radius = color2.y; // You can adjust the radius as needed

	// Calculate the distance from the current pixel to the center of the circle
	float distance = length(In.TexCoord.xy - center);

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
