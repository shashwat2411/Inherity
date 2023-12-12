
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s1);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;
	
	float4 bloom = 0.0f;

	//��ʃT�C�Y X960 Y540�Ƃ����ꍇ
	for (int y = (int)color.x; y <= (int)color.y; y++)	//�㉺�́}12�s�N�Z���͈̔�
	{
		for (int x = (int)color.z; x <= (int)color.w; x++)	//���E�́}3�s�N�Z���͈̔�
		{
			//�����̒l��3�𒲐߂���ƌ��͈̔͂��ω�����
			float2 texCoord = In.TexCoord + float2(x * color2.x / (960.0f*dummy.x), y * color2.y / (540.0f*dummy.x)); //�e�N�X�`�����W�����炷
			float4 color = g_Texture.Sample(g_SamplerState, texCoord);

			//���x���v�Z
			float bright = color.r * 0.299f + color.g * 0.587f + color.b * 0.114f;

			//���x��臒l0.7���傫���A�������Ώۂ̃s�N�Z���łȂ�
			if (bright > color2.z && (x != 0 || y != 0))
			{
				bloom += color / (x * x + y * y) * color2.w;
			}
		}
	}

	outDiffuse.rgb += bloom.rgb;
	outDiffuse.a = 1.0f;
	//outDiffuse.rgb *= Light.Diffuse.rgb;
}