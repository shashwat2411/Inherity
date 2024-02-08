
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_DissolveTexture : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 normal = normalize(In.Normal);

	outDiffuse = In.Diffuse;

	if (outDiffuse.r <= 0.0f && outDiffuse.g >= 1.0f && outDiffuse.b <= 0.0f)
	{
		outDiffuse.rgb = color3.rgb;
	}

	if (color2.r > 0.0f)
	{
		//r ÇÃêFÇæÇØÇãÅÇﬂÇÈ
		float dissolveValue = g_DissolveTexture.Sample(g_SamplerState, In.TexCoord);
		float threshold = dissolveThreshold * (1.0f + dissolveRange) - dissolveRange;
		float rate = saturate((dissolveValue - threshold) / dissolveRange);

		outDiffuse.a = rate;

		outDiffuse.rgb = lerp(outDiffuse.rgb, color.rgb, 1.0f - pow(rate, 12));
	}
	
	if (outDiffuse.a < 0.01f)
	{
		discard;
		return;
	}
	else
	{

		float light = -dot(normal.xyz, Light.Direction.xyz);

		if (light > 0.7f) { light = 1.0f; }
		else if (light > 0.4f) { light = 0.7f; }
		else { light = 0.5f; }

		outDiffuse.rgb *= saturate(light);

		float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
		eyev = normalize(eyev);

		float d = dot(eyev, normal.xyz);

		if (d > -0.4f) { outDiffuse.rgb *= 0.0; }
	}
}
