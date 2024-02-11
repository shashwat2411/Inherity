
#include "common.hlsl"

Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//Variables
	float radius = color2.y; //radius of the map
	float outline = color2.z; //map outline area
	float playerRadius = color2.a; //player area
	float playerOutline = playerRadius * 1.5f; //player outline area

	float2 center = float2(0.5f, 0.5f); //Center of the texture
	float distance = length(In.TexCoord.xy - center.xy); //distance of the pixel from the center

	bool moveon = true;

	if (distance <= playerRadius) { outDiffuse.rgb = float3(1.0f, 0.0f, 0.0f); moveon = false; }
	else if (distance < playerOutline) { outDiffuse.rgb = float3(0.0f, 0.0f, 0.0f); moveon = false; }

	if (moveon == true)
	{
		//float2 texCoord = float2(In.TexCoord.x + color.x - center.x, In.TexCoord.y - color.z - center.y);
		float2 texCoord = In.TexCoord.xy - color.xz - center.xy;
		texCoord = texCoord * 0.5f + 0.5f;


		//Sampling
		if (distance <= radius)
		{
			if ((texCoord.x <= 1.0f && texCoord.x >= 0.0f) && (texCoord.y <= 1.0f && texCoord.y >= 0.0f))
			{
				outDiffuse.rgb = g_Texture.Sample(g_SamplerState, texCoord.xy).rgb;
			}
			else { outDiffuse.rgb = color3.rgb; }
		}
		else if (distance < (radius + outline)) { outDiffuse.rgb = float3(0.0f, 0.0f, 0.0f); }
		else { discard; }

	}

	outDiffuse.a = 1.0f;
}
