
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float2 uv = In.TexCoord;
	float2 screen = float2(960, 540);

	float pixel = 0.1f;

	uv *= screen;	//UV�l���X�N���[�����W�֕ϊ�����
	uv /= pixel;	//��`�̃T�C�Y�Ŋ���
	uv = floor(uv);	//�������𖕏�
	uv *= pixel;		//�X�N���[�����W�ɖ߂�
	uv /= screen;	//�X�N���[�����W����UV�l�֕ϊ�����

	outDiffuse = g_Texture.Sample(g_SamplerState, uv);
	outDiffuse *= In.Diffuse;
	outDiffuse.a = 1.0f;
}
