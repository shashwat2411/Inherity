#include "component.h"
#include "material.h"

float g_FieldHeight[21][21] = 
{
	{ 0.0f, 0.0f, 0.0f, 2.0f, 1.0f, 1.0f, 2.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 2.0f, 1.0f, 3.0f, 5.0f, 1.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 2.0f, 3.0f, 3.0f, 0.0f, 3.0f, 1.0f, 2.0f, 2.0f, 2.0f, 1.0f, 2.0f, 0.0f, 3.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 2.0f, 3.0f, 3.0f, 0.0f, 5.0f, 1.0f, 2.0f, 2.0f, 2.0f, 1.0f, 2.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 2.0f, 3.0f, 5.0f, 0.0f, 7.0f, 1.0f, 2.0f, 2.0f, 2.0f, 1.0f, 2.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f, 3.0f, 5.0f, 0.0f, 7.0f, 1.0f, 2.0f, 2.0f, 2.0f, 1.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 2.0f, 0.0f, 0.0f, 3.0f, 5.0f, 0.0f, 7.0f, 1.0f, 2.0f, 2.0f, 2.0f, 1.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 2.0f, 0.0f, 0.0f, 3.0f, 3.0f, 0.0f, 5.0f, 1.0f, 2.0f, 2.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 2.0f, 0.0f, 0.0f, 3.0f, 3.0f, 0.0f, 5.0f, 1.0f, 2.0f, 2.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 2.0f, 0.0f, 0.0f, 4.0f, 3.0f, 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 2.0f, 0.0f, 0.0f, 4.0f, 1.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 2.0f, 0.0f, 0.0f, 4.0f, 1.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 2.0f, 0.0f, 0.0f, 4.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 2.0f, 0.0f, 0.0f, 4.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 5.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 2.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 5.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 2.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 1.0f, 0.0f, 2.0f, 1.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 5.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 1.0f, 0.0f, 2.0f, 1.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 5.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 2.0f, 1.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 5.0f, 1.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 2.0f, 1.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 5.0f, 1.0f, 0.0f, 1.0f },
};

void MeshField::RecreateField()
{
	//gameObject->GetMaterial<DefaultMaterial>()->SetTexture(TextureReader::GetReadTexture(TextureReader::FIELD_NM_T), 1);
	{
		// ���_�o�b�t�@����
		{
			for (int x = 0; x <= TILES; x++)
			{
				for (int z = 0; z <= TILES; z++)
				{
					m_Vertex[x][z].Position = D3DXVECTOR3((x - TILES / 2) * Size.x, g_FieldHeight[z][x], (z - TILES / 2) * -Size.y);
					m_Vertex[x][z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//�@���x�N�g��
					m_Vertex[x][z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
					//m_Vertex[x][z].TexCoord = D3DXVECTOR2(x * 0.5f, z * 0.5f);
					m_Vertex[x][z].TexCoord = D3DXVECTOR2(x * (TexCoord.x / (float)TILES), z * (TexCoord.y / (float)TILES));
				}
			}

			//�@���x�N�g���Z�o
			for (int x = 1; x <= (TILES - 1); x++)
			{
				for (int z = 1; z <= (TILES - 1); z++)
				{
					D3DXVECTOR3 vx, vz, vn;
					vx = m_Vertex[x + 1][z].Position - m_Vertex[x - 1][z].Position;
					vz = m_Vertex[x][z - 1].Position - m_Vertex[x][z + 1].Position;

					D3DXVec3Cross(&vn, &vz, &vx);
					D3DXVec3Normalize(&vn, &vn);
					m_Vertex[x][z].Normal = vn;
				}
			}


			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(VERTEX_3D) * (TILES + 1) * (TILES + 1);
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = m_Vertex;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &VertexBuffer);
		}




		// �C���f�b�N�X�o�b�t�@����
		{
			unsigned int index[((TILES + 2) * 2) * TILES - 2];

			int i = 0;
			for (int x = 0; x < TILES; x++)
			{
				for (int z = 0; z < (TILES + 1); z++)
				{
					index[i] = x * (TILES + 1) + z;
					i++;

					index[i] = (x + 1) * (TILES + 1) + z;
					i++;
				}

				if (x == (TILES - 1))
					break;

				index[i] = (x + 1) * (TILES + 1) + TILES;
				i++;

				index[i] = (x + 1) * (TILES + 1);
				i++;
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * (((TILES + 2) * 2) * TILES - 2);
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &IndexBuffer);
		}
	}
}

void MeshField::Start()
{
	TexCoord = D3DXVECTOR2(1.0f, 1.0f);
	Size = D3DXVECTOR2(5.0f, 5.0f);

	gameObject->AddMaterial<FieldDefaultMaterial>();

	gameObject->SetDepthShadow(true);
	gameObject->SetDepth(false);
}

void MeshField::End()
{
	if (VertexBuffer != nullptr)
	{
		VertexBuffer->Release();
	}

	if (IndexBuffer != nullptr)
	{
		IndexBuffer->Release();
	}
}


void MeshField::Update()
{

}


void MeshField::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(gameObject->GetVertexLayout());

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(gameObject->GetVertexShader(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(gameObject->GetPixelShader(), NULL, 0);

	D3DXVECTOR3 Scale = gameObject->transform->Scale;
	D3DXVECTOR3 Position = gameObject->transform->Position;
	D3DXVECTOR3 Rotation = gameObject->transform->Rotation;

	// �}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;

	D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y, Rotation.x, Rotation.z);
	D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);

	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);


	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof( VERTEX_3D );
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers( 0, 1, &VertexBuffer, &stride, &offset );

	// �C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer( IndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory( &material, sizeof(material) );
	material.Diffuse = gameObject->GetColor();
	material.TextureEnable = true;
	Renderer::SetMaterial( material );


	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->DrawIndexed(((TILES + 2) * 2) * TILES - 2, 0, 0);

}

float MeshField::GetHeight(D3DXVECTOR3 position)
{
	int x, z;

	//�u���b�N�ԍ��Z�o
	x = position.x / 5.0f + 10.0f;
	z = position.z / -5.0f + 10.0f;

	D3DXVECTOR3 pos[4];

	pos[0] = m_Vertex[x][z].Position;
	pos[1] = m_Vertex[x + 1][z].Position;
	pos[2] = m_Vertex[x][z + 1].Position;
	pos[3] = m_Vertex[x + 1][z + 1].Position;

	D3DXVECTOR3 v12, v1p, c;
	v12 = pos[2] - pos[1];
	v1p = position - pos[1];

	D3DXVec3Cross(&c, &v12, &v1p);

	float py;
	D3DXVECTOR3 n;
	if (c.y > 0.0f)
	{
		//���|���S��
		D3DXVECTOR3 v10;
		v10 = pos[0] - pos[1];
		D3DXVec3Cross(&n, &v10, &v12);
	}
	else
	{
		//�E�|���S��
		D3DXVECTOR3 v13;
		v13 = pos[3] - pos[1];
		D3DXVec3Cross(&n, &v12, &v13);
	}

	//�����擾
	py = -((position.x - pos[1].x) * n.x + (position.z - pos[1].z) * n.z) / n.y + pos[1].y;

	return py;
}