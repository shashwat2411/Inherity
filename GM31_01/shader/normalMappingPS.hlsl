#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureNormalMap : register(t1);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 normalMap = g_TextureNormalMap.Sample(g_SamplerState, In.TexCoord);
	normalMap = (normalMap * 2.0f) - 1.0f;

	float4 n = normalize(In.Normal);
	float4 t = normalize(In.Tangent);
	float4 b = normalize(In.Binormal);
	float4 normal;
	normal.xyz = t.xyz * normalMap.x + b.xyz * normalMap.y + n.xyz * normalMap.z;
	normal.w = 0.0f;
	normal = normalize(normal);


	//�����̌����Ɖ�]�ς݂̖@���Ƃœ��ς��v�Z���A�����𔽓]���ċP�x�����߂�
	float light = -dot(Light.Direction.xyz, normal.xyz);
	//�O�a���Z(0.0�`1.0�̒l�ɃN�����v����)
	light = saturate(light);

	//�e�N�X�`������F���T���v�����O����
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);

	//�e�N�X�`���̐F�ɋP�x����Z����
	outDiffuse.rgb *= light;

	//�����x�͋P�x�ɉe�����Ȃ��̂ł��̂܂܎g��
	outDiffuse.a *= In.Diffuse.a;

	//�u�����t�H�����ʔ���
	//�����x�N�g�����쐬����i�J��������s�N�Z���֌������x�N�g���j
	float3 evev = In.WorldPosition.xyz - CameraPosition.xyz;
	evev = normalize(evev);
	//�n�[�t�x�N�g�����쐬����i�����x�N�g���{���C�g�x�N�g���j
	float3 halfv = evev + Light.Direction.xyz;
	halfv = normalize(halfv);

	float specular = -dot(halfv, normal.xyz);
	saturate(specular);
	specular = pow(specular, 5);
	outDiffuse.rgb = saturate(outDiffuse.rgb + specular);

	outDiffuse *= color;
}