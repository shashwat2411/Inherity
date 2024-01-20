
#include "common.hlsl"



void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;//matrix�^�̕ϐ�wvp���`����

	//wvp = ���[���h�ϊ��s��~�r���[�ϊ��s��
	wvp = mul(World, View);
	//wvp = wvp �~ �v���W�F�N�V�����ϊ��s��
	wvp = mul(wvp, Projection);

	//���͂��ꂽ���_���W��ϊ����ďo�͐�֑������
	Out.Position = mul(In.Position, wvp);

	//���͂��ꂽUV���W���o�͐�֑������
	Out.TexCoord = In.TexCoord;

	//���_�̖@�������[���h�s��ŕϊ�����
	float4 worldNormal, normal;

	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	Out.Normal = worldNormal;
	Out.Tangent = float4(0.0, 0.0, 0.0, 0.0);
	Out.Binormal = float4(0.0, 0.0, 0.0, 0.0);

	float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	light = saturate(light);
	//���_�J���[�͂��̂܂܃R�s�[
	Out.Diffuse = In.Diffuse * Material.Diffuse * light * Light.Diffuse;
	Out.Diffuse += In.Diffuse * Material.Ambient * Light.Ambient;
	Out.Diffuse += Material.Emission;
	Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;

	Out.WorldPosition = mul(In.Position, World);
	Out.ShadowPosition = float4(0.0f, 0.0f, 0.0f, 0.0f);
}
