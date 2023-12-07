#include "main.h"
#include "renderer.h"
#include "animationModel.h"
#include "functions.h"
#include "modelReader.h"


#define INTERPOLATE


void AnimationModel::Draw()
{
	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];


		// �}�e���A���ݒ�
		aiString texture;
		aiColor3D diffuse;
		float opacity;

		aiMaterial* aimaterial = m_AiScene->mMaterials[mesh->mMaterialIndex];
		aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texture);
		aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		aimaterial->Get(AI_MATKEY_OPACITY, opacity);

		if (texture == aiString(""))
		{
			material.TextureEnable = false;
		}
		else
		{
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[texture.data]);
			material.TextureEnable = true;
		}

		material.Diffuse = D3DXCOLOR(diffuse.r, diffuse.g, diffuse.b, opacity);
		material.Ambient = material.Diffuse;
		Renderer::SetMaterial(material);


		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer[m], &stride, &offset);

		// �C���f�b�N�X�o�b�t�@�ݒ�
		Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		// �|���S���`��
		Renderer::GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}




void AnimationModel::Load( const char *FileName )
{
	const std::string modelPath( FileName );

	m_AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(m_AiScene);

	m_VertexBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];


	//�ό`�㒸�_�z�񐶐�
	m_DeformVertex = new std::vector<DEFORM_VERTEX>[m_AiScene->mNumMeshes];

	//�ċA�I�Ƀ{�[������
	CreateBone(m_AiScene->mRootNode);



	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		// ���_�o�b�t�@����
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = D3DXVECTOR3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = D3DXVECTOR3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexCoord = D3DXVECTOR2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

				vertex[v].Tangent = D3DXVECTOR3(mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z);
				vertex[v].Binormal = D3DXVECTOR3(mesh->mBitangents[v].x, mesh->mBitangents[v].y, mesh->mBitangents[v].z);
			}

			//for (unsigned int vc = 0; vc < mesh->mNumVertices; vc += 3)
			//{
			//	CalcTangentAndBinormal
			//	(
			//		&vertex[vc + 0].Position, &vertex[vc + 0].TexCoord,
			//		&vertex[vc + 1].Position, &vertex[vc + 1].TexCoord,
			//		&vertex[vc + 2].Position, &vertex[vc + 2].TexCoord,
			//		&vertex[vc + 0].Tangent,
			//		&vertex[vc + 0].Binormal
			//	);
			//	vertex[vc + 1].Tangent  = vertex[vc + 0].Tangent;
			//	vertex[vc + 1].Binormal = vertex[vc + 0].Binormal;
			//	vertex[vc + 2].Tangent  = vertex[vc + 0].Tangent;
			//	vertex[vc + 2].Binormal = vertex[vc + 0].Binormal;
			//}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[m]);

			delete[] vertex;
		}


		// �C���f�b�N�X�o�b�t�@����
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer[m]);

			delete[] index;
		}



		//�ό`�㒸�_�f�[�^������
		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{
			DEFORM_VERTEX deformVertex;
			deformVertex.Position = mesh->mVertices[v];
			deformVertex.Normal = mesh->mNormals[v];
			deformVertex.BoneNum = 0;

			for (unsigned int b = 0; b < 4; b++)
			{
				deformVertex.BoneName[b] = "";
				deformVertex.BoneWeight[b] = 0.0f;
			}

			m_DeformVertex[m].push_back(deformVertex);
		}


		//�{�[���f�[�^������
		for (unsigned int b = 0; b < mesh->mNumBones; b++)
		{
			aiBone* bone = mesh->mBones[b];

			m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

			//�ό`�㒸�_�Ƀ{�[���f�[�^�i�[
			for (unsigned int w = 0; w < bone->mNumWeights; w++)
			{
				aiVertexWeight weight = bone->mWeights[w];

				int num = m_DeformVertex[m][weight.mVertexId].BoneNum;

				m_DeformVertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
				m_DeformVertex[m][weight.mVertexId].BoneName[num] = bone->mName.C_Str();
				m_DeformVertex[m][weight.mVertexId].BoneNum++;

				assert(m_DeformVertex[m][weight.mVertexId].BoneNum <= 4);
			}
		}
	}



	//�e�N�X�`���ǂݍ���
	for(unsigned int i = 0; i < m_AiScene->mNumTextures; i++)
	{
		aiTexture* aitexture = m_AiScene->mTextures[i];

		ID3D11ShaderResourceView* texture;

		D3DX11CreateShaderResourceViewFromMemory(
			Renderer::GetDevice(),
			(const unsigned char*)aitexture->pcData,
			aitexture->mWidth,
			nullptr,
			nullptr,
			&texture,
			nullptr);

		m_Texture[aitexture->mFilename.data] = texture;
	}



}



void AnimationModel::LoadAnimation( const char *FileName, const char *Name )
{

	m_Animation[Name] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(m_Animation[Name]);
	over = false;
}


void AnimationModel::LoadAnimation(const char *Name)
{
	m_Animation[Name] = ModelReader::GetAnimation(Name);
	over = false;
}

void AnimationModel::CreateBone(aiNode* node)
{
	BONE bone;

	m_Bone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}

}


void AnimationModel::Unload()
{
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		m_VertexBuffer[m]->Release();
		m_IndexBuffer[m]->Release();
	}

	delete[] m_VertexBuffer;
	delete[] m_IndexBuffer;

	delete[] m_DeformVertex;


	for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : m_Texture)
	{
		if (pair.second != nullptr) { pair.second->Release(); }
	}



	aiReleaseImport(m_AiScene);


	for (std::pair<const std::string, const aiScene*> pair : m_Animation)
	{
		pair.second = nullptr;
	}

}





void AnimationModel::Update(const char *AnimationName1, int Frame1, const char* AnimationName2, int Frame2, float BlendRate, float time)
{
	if (m_Animation.count(AnimationName1) == 0) { return; }
	if (!m_Animation[AnimationName1]->HasAnimations()) { return; }

	if (m_Animation.count(AnimationName2) == 0) { return; }
	if (!m_Animation[AnimationName2]->HasAnimations()) { return; }

	//�A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
	aiAnimation* animation1 = m_Animation[AnimationName1]->mAnimations[0];
	aiAnimation* animation2 = m_Animation[AnimationName2]->mAnimations[0];

	for (auto pair : m_Bone)
	{
		BONE* bone = &m_Bone[pair.first];

		aiNodeAnim* nodeAnim1 = nullptr;
		for (unsigned int c = 0; c < animation1->mNumChannels; c++)
		{
			if (animation1->mChannels[c]->mNodeName == aiString(pair.first))
			{
				nodeAnim1 = animation1->mChannels[c];
				break;
			}
		}

		aiNodeAnim* nodeAnim2 = nullptr;
		for (unsigned int c = 0; c < animation2->mNumChannels; c++)
		{
			if (animation2->mChannels[c]->mNodeName == aiString(pair.first))
			{
				nodeAnim2 = animation2->mChannels[c];
				break;
			}
		}

		int f;
		int f1;

		float frame1 = (float)Frame1;
		float frame2 = (float)Frame2;
		float dt = (time - frame1) / (frame2 - frame1);

		aiQuaternion rot_a;
		aiVector3D pos_a;
		if (nodeAnim1)
		{
#ifndef INTERPOLATE
			{
				f = Frame2 % nodeAnim1->mNumRotationKeys;//�ȈՎ���
				rot_a = nodeAnim1->mRotationKeys[f].mValue;

				f = Frame2 % nodeAnim1->mNumPositionKeys;//�ȈՎ���
				pos_a = nodeAnim1->mPositionKeys[f].mValue;
			}
#else

			aiQuaternion rot_a1;
			aiQuaternion rot_a2;

			aiVector3D pos_a1;
			aiVector3D pos_a2;
			//���`�ۊǂ̑��Y�������ł��
			//1
			f = Frame1 % nodeAnim1->mNumRotationKeys;//�ȈՎ���
			rot_a1 = nodeAnim1->mRotationKeys[f].mValue;

			f = Frame1 % nodeAnim1->mNumPositionKeys;//�ȈՎ���
			pos_a1 = nodeAnim1->mPositionKeys[f].mValue;

			//2
			int f_max = ((f + 1) >= (nodeAnim1->mNumRotationKeys) ? 0 : (f + 1));

			f = Frame1 % nodeAnim1->mNumRotationKeys;//�ȈՎ���
			f1 = (f + 1) % nodeAnim1->mNumRotationKeys;
			rot_a2 = nodeAnim1->mRotationKeys[f1].mValue;

			f = Frame1 % nodeAnim1->mNumPositionKeys;//�ȈՎ���
			f1 = (f + 1) % nodeAnim1->mNumRotationKeys;
			pos_a2 = nodeAnim1->mPositionKeys[f1].mValue;


			//rot_a.x = rot_a1.x + (dt * (rot_a2.x - rot_a1.x));
			//rot_a.y = rot_a1.y + (dt * (rot_a2.y - rot_a1.y));
			//rot_a.z = rot_a1.z + (dt * (rot_a2.z - rot_a1.z));
			//rot_a.w = rot_a1.w + (dt * (rot_a2.w - rot_a1.w));
			aiQuaternion::Interpolate(rot_a, rot_a1, rot_a2, dt);

			pos_a.x = pos_a1.x + (dt * (pos_a2.x - pos_a1.x));
			pos_a.y = pos_a1.y + (dt * (pos_a2.y - pos_a1.y));
			pos_a.z = pos_a1.z + (dt * (pos_a2.z - pos_a1.z));
#endif
		}

		aiQuaternion rot_b;
		aiVector3D pos_b;
		if (nodeAnim2)
		{
#ifndef INTERPOLATE
			{
				f = Frame2 % nodeAnim2->mNumRotationKeys;//�ȈՎ���
				rot_b = nodeAnim2->mRotationKeys[f].mValue;

				f = Frame2 % nodeAnim2->mNumPositionKeys;//�ȈՎ���
				pos_b = nodeAnim2->mPositionKeys[f].mValue;
			}

#else
			aiQuaternion rot_b1;
			aiQuaternion rot_b2;

			aiVector3D pos_b1;
			aiVector3D pos_b2;
			//1
			f = Frame2 % nodeAnim2->mNumRotationKeys;//�ȈՎ���
			rot_b1 = nodeAnim2->mRotationKeys[f].mValue;

			f = Frame2 % nodeAnim2->mNumPositionKeys;//�ȈՎ���
			pos_b1 = nodeAnim2->mPositionKeys[f].mValue;

			//2
			f = Frame2 % nodeAnim2->mNumRotationKeys;//�ȈՎ���
			f1 = (f + 1) % nodeAnim2->mNumRotationKeys;
			rot_b2 = nodeAnim2->mRotationKeys[f1].mValue;

			f = Frame2 % nodeAnim2->mNumPositionKeys;//�ȈՎ���
			f1 = (f + 1) % nodeAnim2->mNumRotationKeys;
			pos_b2 = nodeAnim2->mPositionKeys[f1].mValue;

			//rot_b.x = rot_b1.x + (dt * (rot_b2.x - rot_b1.x));
			//rot_b.y = rot_b1.y + (dt * (rot_b2.y - rot_b1.y));
			//rot_b.z = rot_b1.z + (dt * (rot_b2.z - rot_b1.z));
			//rot_b.w = rot_b1.w + (dt * (rot_b2.w - rot_b1.w));
			aiQuaternion::Interpolate(rot_b, rot_b1, rot_b2, dt);

			pos_b.x = pos_b1.x + (dt * (pos_b2.x - pos_b1.x));
			pos_b.y = pos_b1.y + (dt * (pos_b2.y - pos_b1.y));
			pos_b.z = pos_b1.z + (dt * (pos_b2.z - pos_b1.z));
#endif

			//mNumRotationKeys is the maximum number of keys in the animation;
			if (Frame2 % nodeAnim2->mNumRotationKeys == 0 && BlendRate >= 1.0f) { over = true; }
			else { over = false; }
		}

		//���`�ۊ�
		aiVector3D pos = (pos_a * (1.0f - BlendRate) + pos_b * BlendRate);
		aiQuaternion rot;
		aiQuaternion::Interpolate(rot, rot_a, rot_b, BlendRate);
		
		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}




	//�ċA�I�Ƀ{�[���}�g���N�X���X�V
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion((float)AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);


	//���_�ϊ��iCPU�X�L�j���O�j���@GPU�X�L�j���O�@(���_�V�F�[�_�[�A�R���s���[�^�V�F�[�_�[)
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		D3D11_MAPPED_SUBRESOURCE ms;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer[m], 0,
			D3D11_MAP_WRITE_DISCARD, 0, &ms);

		VERTEX_3D *vertex = (VERTEX_3D*)ms.pData;

		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{
			DEFORM_VERTEX *deformVertex = &m_DeformVertex[m][v];

			aiMatrix4x4 matrix[4];
			aiMatrix4x4 outMatrix;
			matrix[0] = m_Bone[deformVertex->BoneName[0]].Matrix;
			matrix[1] = m_Bone[deformVertex->BoneName[1]].Matrix;
			matrix[2] = m_Bone[deformVertex->BoneName[2]].Matrix;
			matrix[3] = m_Bone[deformVertex->BoneName[3]].Matrix;

			//�E�F�C�g���l�����ă}�g���N�X�Z�o
			//outMatrix = matrix[0] * deformVertex->BoneWeight[0]
			//			+ matrix[1] * deformVertex->BoneWeight[1]
			//			+ matrix[2] * deformVertex->BoneWeight[2]
			//			+ matrix[3] * deformVertex->BoneWeight[3];
			{
				outMatrix.a1 = matrix[0].a1 * deformVertex->BoneWeight[0]
							+ matrix[1].a1 * deformVertex->BoneWeight[1]
							+ matrix[2].a1 * deformVertex->BoneWeight[2]
							+ matrix[3].a1 * deformVertex->BoneWeight[3];

				outMatrix.a2 = matrix[0].a2 * deformVertex->BoneWeight[0]
							+ matrix[1].a2 * deformVertex->BoneWeight[1]
							+ matrix[2].a2 * deformVertex->BoneWeight[2]
							+ matrix[3].a2 * deformVertex->BoneWeight[3];

				outMatrix.a3 = matrix[0].a3 * deformVertex->BoneWeight[0]
							+ matrix[1].a3 * deformVertex->BoneWeight[1]
							+ matrix[2].a3 * deformVertex->BoneWeight[2]
							+ matrix[3].a3 * deformVertex->BoneWeight[3];

				outMatrix.a4 = matrix[0].a4 * deformVertex->BoneWeight[0]
							+ matrix[1].a4 * deformVertex->BoneWeight[1]
							+ matrix[2].a4 * deformVertex->BoneWeight[2]
							+ matrix[3].a4 * deformVertex->BoneWeight[3];



				outMatrix.b1 = matrix[0].b1 * deformVertex->BoneWeight[0]
							+ matrix[1].b1 * deformVertex->BoneWeight[1]
							+ matrix[2].b1 * deformVertex->BoneWeight[2]
							+ matrix[3].b1 * deformVertex->BoneWeight[3];

				outMatrix.b2 = matrix[0].b2 * deformVertex->BoneWeight[0]
					+ matrix[1].b2 * deformVertex->BoneWeight[1]
					+ matrix[2].b2 * deformVertex->BoneWeight[2]
					+ matrix[3].b2 * deformVertex->BoneWeight[3];

				outMatrix.b3 = matrix[0].b3 * deformVertex->BoneWeight[0]
					+ matrix[1].b3 * deformVertex->BoneWeight[1]
					+ matrix[2].b3 * deformVertex->BoneWeight[2]
					+ matrix[3].b3 * deformVertex->BoneWeight[3];

				outMatrix.b4 = matrix[0].b4 * deformVertex->BoneWeight[0]
					+ matrix[1].b4 * deformVertex->BoneWeight[1]
					+ matrix[2].b4 * deformVertex->BoneWeight[2]
					+ matrix[3].b4 * deformVertex->BoneWeight[3];



				outMatrix.c1 = matrix[0].c1 * deformVertex->BoneWeight[0]
					+ matrix[1].c1 * deformVertex->BoneWeight[1]
					+ matrix[2].c1 * deformVertex->BoneWeight[2]
					+ matrix[3].c1 * deformVertex->BoneWeight[3];

				outMatrix.c2 = matrix[0].c2 * deformVertex->BoneWeight[0]
					+ matrix[1].c2 * deformVertex->BoneWeight[1]
					+ matrix[2].c2 * deformVertex->BoneWeight[2]
					+ matrix[3].c2 * deformVertex->BoneWeight[3];

				outMatrix.c3 = matrix[0].c3 * deformVertex->BoneWeight[0]
					+ matrix[1].c3 * deformVertex->BoneWeight[1]
					+ matrix[2].c3 * deformVertex->BoneWeight[2]
					+ matrix[3].c3 * deformVertex->BoneWeight[3];

				outMatrix.c4 = matrix[0].c4 * deformVertex->BoneWeight[0]
					+ matrix[1].c4 * deformVertex->BoneWeight[1]
					+ matrix[2].c4 * deformVertex->BoneWeight[2]
					+ matrix[3].c4 * deformVertex->BoneWeight[3];



				outMatrix.d1 = matrix[0].d1 * deformVertex->BoneWeight[0]
					+ matrix[1].d1 * deformVertex->BoneWeight[1]
					+ matrix[2].d1 * deformVertex->BoneWeight[2]
					+ matrix[3].d1 * deformVertex->BoneWeight[3];

				outMatrix.d2 = matrix[0].d2 * deformVertex->BoneWeight[0]
					+ matrix[1].d2 * deformVertex->BoneWeight[1]
					+ matrix[2].d2 * deformVertex->BoneWeight[2]
					+ matrix[3].d2 * deformVertex->BoneWeight[3];

				outMatrix.d3 = matrix[0].d3 * deformVertex->BoneWeight[0]
					+ matrix[1].d3 * deformVertex->BoneWeight[1]
					+ matrix[2].d3 * deformVertex->BoneWeight[2]
					+ matrix[3].d3 * deformVertex->BoneWeight[3];

				outMatrix.d4 = matrix[0].d4 * deformVertex->BoneWeight[0]
					+ matrix[1].d4 * deformVertex->BoneWeight[1]
					+ matrix[2].d4 * deformVertex->BoneWeight[2]
					+ matrix[3].d4 * deformVertex->BoneWeight[3];

			}

			deformVertex->Position = mesh->mVertices[v];
			deformVertex->Position *= outMatrix;


			//�@���ϊ��p�Ɉړ��������폜
			outMatrix.a4 = 0.0f;
			outMatrix.b4 = 0.0f;
			outMatrix.c4 = 0.0f;

			deformVertex->Normal = mesh->mNormals[v];
			deformVertex->Normal *= outMatrix;

			deformVertex->Tangent = mesh->mTangents[v];
			deformVertex->Tangent *= outMatrix;

			deformVertex->Binormal = mesh->mBitangents[v];
			deformVertex->Binormal *= outMatrix;


			//���_�o�b�t�@�֏�������
			vertex[v].Position.x = deformVertex->Position.x;
			vertex[v].Position.y = deformVertex->Position.y;
			vertex[v].Position.z = deformVertex->Position.z;

			//���_��Normal��ۑ�����ꏊ
			vertex[v].Normal.x = deformVertex->Normal.x;
			vertex[v].Normal.y = deformVertex->Normal.y;
			vertex[v].Normal.z = deformVertex->Normal.z;

			vertex[v].Tangent.x = deformVertex->Tangent.x;
			vertex[v].Tangent.y = deformVertex->Tangent.y;
			vertex[v].Tangent.z = deformVertex->Tangent.z;

			vertex[v].Binormal.x = deformVertex->Binormal.x;
			vertex[v].Binormal.y = deformVertex->Binormal.y;
			vertex[v].Binormal.z = deformVertex->Binormal.z;

			vertex[v].TexCoord.x = mesh->mTextureCoords[0][v].x;
			vertex[v].TexCoord.y = mesh->mTextureCoords[0][v].y;

			vertex[v].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		}
		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer[m], 0);
	}
}


void AnimationModel::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	BONE* bone = &m_Bone[node->mName.C_Str()];
	//�}�g���N�X�̏�Z���Ԃɒ���
	aiMatrix4x4 worldMatrix;

	worldMatrix *= matrix;
	worldMatrix *= bone->AnimationMatrix;

	bone->Matrix = worldMatrix;//���������K�v
	bone->worldMatrix = worldMatrix;

	bone->Matrix *= bone->OffsetMatrix;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}

void AnimationModel::CalcTangentAndBinormal(
	D3DXVECTOR3 * p0, D3DXVECTOR2 * uv0,
	D3DXVECTOR3 * p1, D3DXVECTOR2 * uv1,
	D3DXVECTOR3 * p2, D3DXVECTOR2 * uv2,
	D3DXVECTOR3 * outTangent, D3DXVECTOR3 * outBinormal)
{
	D3DXVECTOR3 CP0[3] = {//���_1���쐬����
	D3DXVECTOR3(p0->x, uv0->x, uv0->y),
	D3DXVECTOR3(p0->y, uv0->x, uv0->y),
	D3DXVECTOR3(p0->z, uv0->x, uv0->y),
	};
	D3DXVECTOR3 CP1[3] = {//���_2���쐬����
	D3DXVECTOR3(p1->x, uv1->x, uv1->y),
	D3DXVECTOR3(p1->y, uv1->x, uv1->y),
	D3DXVECTOR3(p1->z, uv1->x, uv1->y),
	};
	D3DXVECTOR3 CP2[3] = {//���_3���쐬����
	D3DXVECTOR3(p2->x, uv2->x, uv2->y),
	D3DXVECTOR3(p2->y, uv2->x, uv2->y),
	D3DXVECTOR3(p2->z, uv2->x, uv2->y),
	};
	float U[3], V[3];//X,Y,Z�̊e�����ɂ��Čv�Z����
	for (int i = 0; i < 3; ++i)
	{
		D3DXVECTOR3 V1 = CP1[i] - CP0[i];
		D3DXVECTOR3 V2 = CP2[i] - CP1[i];
		D3DXVECTOR3 ABC;
		//V1��V2�̊O�ς���@�����쐬����
		D3DXVec3Cross(&ABC, &V1, &V2);
		if (ABC.x == 0.0f)
		{
			//�k�ރ|���S��
			*outTangent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			*outBinormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			return;
		}
		//�Δ����ŋ��߂�ꂽ�l���
		U[i] = -ABC.y / ABC.x;
		V[i] = -ABC.z / ABC.x;
	}
	//�l����
	*outTangent = D3DXVECTOR3(U[0], U[1], U[2]);
	*outBinormal = D3DXVECTOR3(V[0], V[1], V[2]);
	//���K������
	D3DXVec3Normalize(outTangent, outTangent);
	D3DXVec3Normalize(outBinormal, outBinormal);
}