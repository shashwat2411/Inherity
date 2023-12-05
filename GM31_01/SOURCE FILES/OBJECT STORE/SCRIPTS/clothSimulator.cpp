#include "script.h"
#include "manager.h"
#include "prefab.h"
#include "input.h"

#define F_ZERO(v)			v.x=v.y=v.z=0.0f
#define F_SET(v,px,py,pz)	v.x=px;v.y=py;v.z=pz

#define WIND_FORCE_DEF_X		40.0f			// ���͂w����
#define WIND_FORCE_DEF_Y		20.0f			// ���͂x����
#define WIND_FORCE_DEF_Z		30.0f			// ���͂y����

#define ATT_COEFFICIENT			0.5f			// �����W��(Attenuation)
#define DELTA_TIME				0.1f			// �ϕ��P�ʎ���

#define SPRING_DEFAULT_LENGTH	2.0f			// �o�l���R��
#define SPRING_COEFFICIENT		10.0f			// �o�l�W��
#define SPRING_MASS				2.0f			// �o�l����

bool		g_bOnWind = true;					// ���̓X�C�b�`

D3DXVECTOR3	vec_spr;				// ���q�Q�_�Ԃ̃x�N�g��
D3DXVECTOR3	resistance;				// ��R�̓x�N�g��
D3DXVECTOR3	windforce;				// ���̓x�N�g��
float	f1, f2;					// �o�l�����q�ɗ^����́i�Q��j
int		h1, v1, h2, v2;			// �o�l�[�ɂ��闱�q�̃C���f�b�N�X�i���A�c�ʂQ���j
float	f1x, f1y, f1z;			// ��������o�l�ɔ��������
float	f2x, f2y, f2z;
float	length;					// ���q�Ԃ̋���
float	xx, yy, zz;				// ���q�Ԃ̋����i�������j
int		count;					// �o�l�J�E���g�p
int		i, j;					// ���[�v�J�E���^

void ClothSimulator::Start()
{
	num = TILES + 1;
	mesh = gameObject->GetComponent<MeshField>();

	g_bOnWind = true;			// ���X�C�b�`���n�m
	windForce.status = 0;		// ���̓X�e�[�^�X�̏�����
	cloth.status = 0;		// �z�X�e�[�^�X�̏�����


	windForce.force.x = WIND_FORCE_DEF_X;		// ���͂̏����l��ݒ�
	windForce.force.y = WIND_FORCE_DEF_Y;
	windForce.force.z = WIND_FORCE_DEF_Z;

	{
		// ���q�̏�����
		for (j = 0; j <= TILES; j++) {
			for (i = 0; i <= TILES; i++) {
				// ���W��ݒ�
				cloth.particle[j][i].position.x = SPRING_DEFAULT_LENGTH * (i - TILES / 2);
				cloth.particle[j][i].position.y = -SPRING_DEFAULT_LENGTH * (j - TILES / 2);
				cloth.particle[j][i].position.z = 0.0f;
				F_ZERO(cloth.particle[j][i].velocity);									// ���x������
				F_ZERO(cloth.particle[j][i].acceleration);								// �����x������
				F_ZERO(cloth.particle[j][i].resultant);									// ���͏�����
				F_SET(cloth.particle[j][i].gravity, 0.0f, SPRING_MASS * -9.8f, 0.0f);		// �d�͏�����
				// ����̗��q�̓��b�N��Ԃ�
				cloth.particle[j][i].onLock = (i == 0 && (j == 0 || j == TILES));	// ���b�N
				cloth.particle[j][i].onDrag = false;					// �h���b�O����ĂȂ�
			}
		}

		// �o�l�̏�����
		count = 0;
		for (j = 0; j <= TILES; j++) {
			for (i = 0; i <= TILES; i++) {
				// �o�l�Q�ƃC���f�b�N�X�̐ݒ�
				if (i < TILES) {
					cloth.Spring[count].p1.horz = i;		// �P�ڂ̎Q�Ɨ��q�C���f�b�N�X��ݒ�
					cloth.Spring[count].p1.vert = j;
					cloth.Spring[count].p2.horz = i + 1;	// �Q�ڂ̎Q�Ɨ��q�C���f�b�N�X��ݒ�
					cloth.Spring[count].p2.vert = j;
					// �Q�̗��q�Ԃ̋��������߁A�o�l�̎��R���Ƃ���
					xx = cloth.particle[j][i].position.x - cloth.particle[j][i + 1].position.x;
					yy = cloth.particle[j][i].position.y - cloth.particle[j][i + 1].position.y;
					zz = cloth.particle[j][i].position.z - cloth.particle[j][i + 1].position.z;
					cloth.Spring[count].length = sqrtf(xx*xx + yy * yy + zz * zz);
					count++;
				}
				if (j < TILES) {
					cloth.Spring[count].p1.horz = i;
					cloth.Spring[count].p1.vert = j;
					cloth.Spring[count].p2.horz = i;
					cloth.Spring[count].p2.vert = j + 1;
					xx = cloth.particle[j][i].position.x - cloth.particle[j + 1][i].position.x;
					yy = cloth.particle[j][i].position.y - cloth.particle[j + 1][i].position.y;
					zz = cloth.particle[j][i].position.z - cloth.particle[j + 1][i].position.z;
					cloth.Spring[count].length = sqrtf(xx*xx + yy * yy + zz * zz);
					count++;
				}
				if (i < TILES && j < TILES) {
					cloth.Spring[count].p1.horz = i;
					cloth.Spring[count].p1.vert = j;
					cloth.Spring[count].p2.horz = i + 1;
					cloth.Spring[count].p2.vert = j + 1;
					xx = cloth.particle[j][i].position.x - cloth.particle[j + 1][i + 1].position.x;
					yy = cloth.particle[j][i].position.y - cloth.particle[j + 1][i + 1].position.y;
					zz = cloth.particle[j][i].position.z - cloth.particle[j + 1][i + 1].position.z;
					cloth.Spring[count].length = sqrtf(xx*xx + yy * yy + zz * zz);
					count++;
				}
				if (i > 0 && j < TILES) {
					cloth.Spring[count].p1.horz = i;
					cloth.Spring[count].p1.vert = j;
					cloth.Spring[count].p2.horz = i - 1;
					cloth.Spring[count].p2.vert = j + 1;
					xx = cloth.particle[j][i].position.x - cloth.particle[j + 1][i - 1].position.x;
					yy = cloth.particle[j][i].position.y - cloth.particle[j + 1][i - 1].position.y;
					zz = cloth.particle[j][i].position.z - cloth.particle[j + 1][i - 1].position.z;
					cloth.Spring[count].length = sqrtf(xx*xx + yy * yy + zz * zz);
					count++;
				}
			}
		}
	}
}

void ClothSimulator::End()
{

}

void ClothSimulator::Update()
{
	WindForce();
	ClothSimulation();

	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(mesh->GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
	//for (int i = 0; i <= TILES; i++)
	//{
	//	for (int j = 0; j <= TILES; j++)
	//	{
	//		((vertex + i) + j)->Position = cloth.particle[i][j].position;
	//		((vertex + i) + j)->Diffuse = (D3DXVECTOR4)gameObject->GetColor();
	//	}
	//}

	Renderer::GetDeviceContext()->Unmap(mesh->GetVertexBuffer(), 0);
}

void ClothSimulator::Draw()
{

}

void ClothSimulator::EngineDisplay()
{

}

void ClothSimulator::WindForce()
{
	if (Input::GetKeyTrigger('P')) { g_bOnWind *= !g_bOnWind; }

	if (Input::GetKeyPress(VK_UP)) { windForce.force.y += 1.0f; }
	if (Input::GetKeyPress(VK_DOWN)) { windForce.force.y -= 1.0f; }
	if (Input::GetKeyPress(VK_LEFT)) { windForce.force.x -= 1.0f; }
	if (Input::GetKeyPress(VK_RIGHT)) { windForce.force.x += 1.0f; }
	if (Input::GetKeyPress(VK_PRIOR)) { windForce.force.z -= 1.0f; }
	if (Input::GetKeyPress(VK_NEXT)) { windForce.force.z += 1.0f; }
	if (Input::GetKeyPress(VK_HOME)) { windForce.status = 0; }

}

void ClothSimulator::ClothSimulation()
{
	// ���q�̏���
	for (j = 0; j <= TILES; j++) {
		for (i = 0; i <= TILES; i++) {
			// ���͂��O�ɂ���
			cloth.particle[j][i].resultant.x = 0;
			cloth.particle[j][i].resultant.y = 0;
			cloth.particle[j][i].resultant.z = 0;

			if (!cloth.particle[j][i].onLock && !cloth.particle[j][i].onDrag) {
				// �d�͂����߂�
				F_SET(cloth.particle[j][i].gravity, 0.0f, SPRING_MASS * -0.98f, 0.0f);

				// ��R�͂����߂�
				resistance.x = -cloth.particle[j][i].velocity.x * ATT_COEFFICIENT;
				resistance.y = -cloth.particle[j][i].velocity.y * ATT_COEFFICIENT;
				resistance.z = -cloth.particle[j][i].velocity.z * ATT_COEFFICIENT;

				// ���͂����߂�
				if (g_bOnWind) {//��ON�̏ꍇ�@�����ŕ��͂����
					windforce.x = windForce.force.x + (float)(rand() % 30 - 15);
					windforce.y = windForce.force.y + (float)(rand() % 30 - 15);
					windforce.z = windForce.force.z + (float)(rand() % 30 - 15);

				}
				else {
					windforce.x = 0;
					windforce.y = 0;
					windforce.z = 0;
				}

				// ���́��d�́{��R�́{����
				cloth.particle[j][i].resultant.x = cloth.particle[j][i].gravity.x + resistance.x + windforce.x;
				cloth.particle[j][i].resultant.y = cloth.particle[j][i].gravity.y + resistance.y + windforce.y;
				cloth.particle[j][i].resultant.z = cloth.particle[j][i].gravity.z + resistance.z + windforce.z;
			}
		}
	}

	// �o�l�̏���
	for (i = 0; i < SPRING_NUMS; i++) {
		// �o�l�̎Q�Ɨ��q�i�Q�j���擾
		h1 = cloth.Spring[i].p1.horz;
		v1 = cloth.Spring[i].p1.vert;
		h2 = cloth.Spring[i].p2.horz;
		v2 = cloth.Spring[i].p2.vert;
		// �Q�̗��q�Ԃ̃x�N�g�������߂�
		vec_spr.x = cloth.particle[v1][h1].position.x - cloth.particle[v2][h2].position.x;
		vec_spr.y = cloth.particle[v1][h1].position.y - cloth.particle[v2][h2].position.y;
		vec_spr.z = cloth.particle[v1][h1].position.z - cloth.particle[v2][h2].position.z;
		// ���q�Ԃ̋��������߂�
		length = sqrtf(vec_spr.x * vec_spr.x + vec_spr.y * vec_spr.y + vec_spr.z * vec_spr.z);
		// �����A���R���A�o�l�W�����炩����͂����߂�i�Q�ڂ͋t�����j
		f1 = -SPRING_COEFFICIENT * (length - cloth.Spring[i].length);
		f2 = -f1;
		// �x�N�g���̐����ɗ͂�������
		f1x = f1 * (vec_spr.x / length);
		f1y = f1 * (vec_spr.y / length);
		f1z = f1 * (vec_spr.z / length);
		f2x = f2 * (vec_spr.x / length);
		f2y = f2 * (vec_spr.y / length);
		f2z = f2 * (vec_spr.z / length);
		// ���߂��͂����͂ɉ�����
		if (cloth.particle[v1][h1].onLock == false) {
			cloth.particle[v1][h1].resultant.x += f1x;
			cloth.particle[v1][h1].resultant.y += f1y;
			cloth.particle[v1][h1].resultant.z += f1z;
		}
		if (cloth.particle[v2][h2].onLock == false) {
			cloth.particle[v2][h2].resultant.x += f2x;
			cloth.particle[v2][h2].resultant.y += f2y;
			cloth.particle[v2][h2].resultant.z += f2z;
		}
	}

	for (j = 0; j <= TILES; j++) {
		for (i = 0; i <= TILES; i++) {
			// �Œ肳��Ă���΃X�L�b�v
			if (cloth.particle[j][i].onLock || cloth.particle[j][i].onDrag) continue;

			// ���͂Ǝ��ʂ�������x�����߂�
			cloth.particle[j][i].acceleration.x = cloth.particle[j][i].resultant.x / SPRING_MASS;
			cloth.particle[j][i].acceleration.y = cloth.particle[j][i].resultant.y / SPRING_MASS;
			cloth.particle[j][i].acceleration.z = cloth.particle[j][i].resultant.z / SPRING_MASS;

			// ���x�ɉ����x��������
			cloth.particle[j][i].velocity.x += cloth.particle[j][i].acceleration.x * 1.0f / FRAME_RATE;
			cloth.particle[j][i].velocity.y += cloth.particle[j][i].acceleration.y * 1.0f / FRAME_RATE;
			cloth.particle[j][i].velocity.z += cloth.particle[j][i].acceleration.z * 1.0f / FRAME_RATE;

			// ���x������W���ړ�
			cloth.particle[j][i].position.x += cloth.particle[j][i].velocity.x * Time::deltaTime;
			cloth.particle[j][i].position.y += cloth.particle[j][i].velocity.y * Time::deltaTime;
			cloth.particle[j][i].position.z += cloth.particle[j][i].velocity.z * Time::deltaTime;
		}
	}
}