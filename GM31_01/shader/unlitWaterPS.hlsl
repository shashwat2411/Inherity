

#include "common.hlsl"


Texture2D g_Texture : register(t0);
Texture2D g_TextureMirror : register(t1);
SamplerState g_SamplerState : register(s0);
SamplerState g_SamplerStateClamp : register(s1);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//float4 normal = normalize(In.Normal);

	//float4 textureColor = g_Texture.Sample(g_SamplerState, In.TexCoord);
	//textureColor *= In.Diffuse;
	//textureColor.a *= Material.Diffuse.a;

	//float light = -dot(Light.Direction.xyz, normal.xyz);
	//light = saturate(light);

	//float4 mirror = g_TextureMirror.Sample(g_SamplerState, In.TexCoord);

	////outDiffuse.rgb = 0.6f * mirror.rgb + 0.4 * textureColor.rgb;
	////outDiffuse.a = textureColor.a;
	//outDiffuse = mirror;
	//outDiffuse.a = 1.0f;

	//if (outDiffuse.a > 0.01f)
	//{
	//	outDiffuse.rgb *= float3(1.0f,1.0f,1.0f)*1.0f;
	//}
	//else { discard; }


	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;

	outDiffuse.rgb *= g_TextureMirror.Sample(g_SamplerState, In.TexCoord).rgb * 0.5f;


	outDiffuse *= color;
}

