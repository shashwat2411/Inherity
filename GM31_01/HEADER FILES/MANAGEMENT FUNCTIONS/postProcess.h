#pragma once
#include "main.h"
#include "renderer.h"

class POSTPROCESS
{
private:

	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	//������	�V�F�[�_�[�֘A�̕ϐ���ǉ�
	ID3D11VertexShader* m_VertexShader; //���_�V�F�[�_�[�I�u�W�F�N�g
	ID3D11PixelShader* m_PixelShader; //�s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	ID3D11InputLayout* m_VertexLayout; //���_���C�A�E�g�I�u�W�F�N�g


public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};