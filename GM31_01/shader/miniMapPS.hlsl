
#include "common.hlsl"

Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//Variables
	float clip = color2.x; //amount to clip from the texture
	float radius = color2.y; //radius of the map
	float outline = color2.z; //map outline area
	float playerRadius = color2.a; //player area
	float playerOutline = playerRadius * 1.5f; //player outline area

	float2 center = float2(0.5f, 0.5f); //Center of the texture
	float distance = length(In.TexCoord.xy - center.xy); //distance of the pixel from the center


	float3 coord = (color.rgb) * 0.5f + 0.5f; // texture coordinates of the player

	float2 texCoord = float2(In.TexCoord.x + coord.x, In.TexCoord.y + coord.z);
	texCoord = texCoord.xy * 0.5f + 0.5f;

	//Sampling
	if (distance <= radius) { outDiffuse = g_Texture.Sample(g_SamplerState, texCoord.xy); }
	else if (distance < (radius + outline)) { outDiffuse.rgb = float3(0.0f, 0.0f, 0.0f); }
	else { discard; }

	if(distance <= playerRadius){ outDiffuse.rgb = float3(1.0f, 0.0f, 0.0f); }
	else if (distance < playerOutline) { outDiffuse.rgb = float3(0.0f, 0.0f, 0.0f); }

	outDiffuse.a = 1.0f;

	////Player
	//float x = abs(In.TexCoord.x - coord.x);
	//float y = abs(In.TexCoord.y - coord.z);

	//if (x < outline && y < outline && distance <= outline) { outDiffuse.rgb = float3(0.0f, 0.0f, 0.0f); }
	//if (x < color2.a && y < color2.a && distance <= color2.a) { outDiffuse.rgb = float3(1.0f, 0.0f, 0.0f); }

	//outDiffuse.a = 1.0f;


	//float clip = color2.x;
	//Circle
	//if (In.TexCoord.x > (coord.x + clip) || In.TexCoord.x < (coord.x - clip) && (In.TexCoord.x <= 1.0f && In.TexCoord.x >= 0.0f))
	//{
	//	discard;
	//}
	//if (In.TexCoord.y > (coord.z + clip) || In.TexCoord.y < (coord.z - clip) && (In.TexCoord.y <= 1.0f && In.TexCoord.y >= 0.0f))
	//{
	//	discard;
	//}





}
