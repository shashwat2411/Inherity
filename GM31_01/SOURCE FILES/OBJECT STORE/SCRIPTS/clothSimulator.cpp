#include "script.h"
#include "manager.h"
#include "prefab.h"
#include "input.h"

#define F_ZERO(v)			v.x=v.y=v.z=0.0f
#define F_SET(v,px,py,pz)	v.x=px;v.y=py;v.z=pz

#define WIND_FORCE_DEF_X		40.0f			// 風力Ｘ成分
#define WIND_FORCE_DEF_Y		20.0f			// 風力Ｙ成分
#define WIND_FORCE_DEF_Z		30.0f			// 風力Ｚ成分

#define ATT_COEFFICIENT			0.5f			// 減衰係数(Attenuation)
#define DELTA_TIME				0.1f			// 積分単位時間

#define SPRING_DEFAULT_LENGTH	2.0f			// バネ自然長
#define SPRING_COEFFICIENT		10.0f			// バネ係数
#define SPRING_MASS				2.0f			// バネ質量

bool		g_bOnWind = true;					// 風力スイッチ

D3DXVECTOR3	vec_spr;				// 粒子２点間のベクトル
D3DXVECTOR3	resistance;				// 抵抗力ベクトル
D3DXVECTOR3	windforce;				// 風力ベクトル
float	f1, f2;					// バネが粒子に与える力（２種）
int		h1, v1, h2, v2;			// バネ端にある粒子のインデックス（横、縦別２個分）
float	f1x, f1y, f1z;			// 長さからバネに発生する力
float	f2x, f2y, f2z;
float	length;					// 粒子間の距離
float	xx, yy, zz;				// 粒子間の距離（成分毎）
int		count;					// バネカウント用
int		i, j;					// ループカウンタ

void ClothSimulator::Start()
{
	num = TILES + 1;
	mesh = gameObject->GetComponent<MeshField>();

	g_bOnWind = true;			// 風スイッチをＯＮ
	windForce.status = 0;		// 風力ステータスの初期化
	cloth.status = 0;		// 布ステータスの初期化


	windForce.force.x = WIND_FORCE_DEF_X;		// 風力の初期値を設定
	windForce.force.y = WIND_FORCE_DEF_Y;
	windForce.force.z = WIND_FORCE_DEF_Z;

	{
		// 粒子の初期化
		for (j = 0; j <= TILES; j++) {
			for (i = 0; i <= TILES; i++) {
				// 座標を設定
				cloth.particle[j][i].position.x = SPRING_DEFAULT_LENGTH * (i - TILES / 2);
				cloth.particle[j][i].position.y = -SPRING_DEFAULT_LENGTH * (j - TILES / 2);
				cloth.particle[j][i].position.z = 0.0f;
				F_ZERO(cloth.particle[j][i].velocity);									// 速度初期化
				F_ZERO(cloth.particle[j][i].acceleration);								// 加速度初期化
				F_ZERO(cloth.particle[j][i].resultant);									// 合力初期化
				F_SET(cloth.particle[j][i].gravity, 0.0f, SPRING_MASS * -9.8f, 0.0f);		// 重力初期化
				// 特定の粒子はロック状態へ
				cloth.particle[j][i].onLock = (i == 0 && (j == 0 || j == TILES));	// ロック
				cloth.particle[j][i].onDrag = false;					// ドラッグされてない
			}
		}

		// バネの初期化
		count = 0;
		for (j = 0; j <= TILES; j++) {
			for (i = 0; i <= TILES; i++) {
				// バネ参照インデックスの設定
				if (i < TILES) {
					cloth.Spring[count].p1.horz = i;		// １個目の参照粒子インデックスを設定
					cloth.Spring[count].p1.vert = j;
					cloth.Spring[count].p2.horz = i + 1;	// ２個目の参照粒子インデックスを設定
					cloth.Spring[count].p2.vert = j;
					// ２個の粒子間の距離を求め、バネの自然長とする
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
	// 粒子の処理
	for (j = 0; j <= TILES; j++) {
		for (i = 0; i <= TILES; i++) {
			// 合力を０にする
			cloth.particle[j][i].resultant.x = 0;
			cloth.particle[j][i].resultant.y = 0;
			cloth.particle[j][i].resultant.z = 0;

			if (!cloth.particle[j][i].onLock && !cloth.particle[j][i].onDrag) {
				// 重力を求める
				F_SET(cloth.particle[j][i].gravity, 0.0f, SPRING_MASS * -0.98f, 0.0f);

				// 抵抗力を求める
				resistance.x = -cloth.particle[j][i].velocity.x * ATT_COEFFICIENT;
				resistance.y = -cloth.particle[j][i].velocity.y * ATT_COEFFICIENT;
				resistance.z = -cloth.particle[j][i].velocity.z * ATT_COEFFICIENT;

				// 風力を求める
				if (g_bOnWind) {//風ONの場合　乱数で風力を作る
					windforce.x = windForce.force.x + (float)(rand() % 30 - 15);
					windforce.y = windForce.force.y + (float)(rand() % 30 - 15);
					windforce.z = windForce.force.z + (float)(rand() % 30 - 15);

				}
				else {
					windforce.x = 0;
					windforce.y = 0;
					windforce.z = 0;
				}

				// 合力＝重力＋抵抗力＋風力
				cloth.particle[j][i].resultant.x = cloth.particle[j][i].gravity.x + resistance.x + windforce.x;
				cloth.particle[j][i].resultant.y = cloth.particle[j][i].gravity.y + resistance.y + windforce.y;
				cloth.particle[j][i].resultant.z = cloth.particle[j][i].gravity.z + resistance.z + windforce.z;
			}
		}
	}

	// バネの処理
	for (i = 0; i < SPRING_NUMS; i++) {
		// バネの参照粒子（２個）を取得
		h1 = cloth.Spring[i].p1.horz;
		v1 = cloth.Spring[i].p1.vert;
		h2 = cloth.Spring[i].p2.horz;
		v2 = cloth.Spring[i].p2.vert;
		// ２個の粒子間のベクトルを求める
		vec_spr.x = cloth.particle[v1][h1].position.x - cloth.particle[v2][h2].position.x;
		vec_spr.y = cloth.particle[v1][h1].position.y - cloth.particle[v2][h2].position.y;
		vec_spr.z = cloth.particle[v1][h1].position.z - cloth.particle[v2][h2].position.z;
		// 粒子間の距離を求める
		length = sqrtf(vec_spr.x * vec_spr.x + vec_spr.y * vec_spr.y + vec_spr.z * vec_spr.z);
		// 距離、自然長、バネ係数からかかる力を求める（２つ目は逆方向）
		f1 = -SPRING_COEFFICIENT * (length - cloth.Spring[i].length);
		f2 = -f1;
		// ベクトルの成分に力をかける
		f1x = f1 * (vec_spr.x / length);
		f1y = f1 * (vec_spr.y / length);
		f1z = f1 * (vec_spr.z / length);
		f2x = f2 * (vec_spr.x / length);
		f2y = f2 * (vec_spr.y / length);
		f2z = f2 * (vec_spr.z / length);
		// 求めた力を合力に加える
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
			// 固定されていればスキップ
			if (cloth.particle[j][i].onLock || cloth.particle[j][i].onDrag) continue;

			// 合力と質量から加速度を求める
			cloth.particle[j][i].acceleration.x = cloth.particle[j][i].resultant.x / SPRING_MASS;
			cloth.particle[j][i].acceleration.y = cloth.particle[j][i].resultant.y / SPRING_MASS;
			cloth.particle[j][i].acceleration.z = cloth.particle[j][i].resultant.z / SPRING_MASS;

			// 速度に加速度を加える
			cloth.particle[j][i].velocity.x += cloth.particle[j][i].acceleration.x * 1.0f / FRAME_RATE;
			cloth.particle[j][i].velocity.y += cloth.particle[j][i].acceleration.y * 1.0f / FRAME_RATE;
			cloth.particle[j][i].velocity.z += cloth.particle[j][i].acceleration.z * 1.0f / FRAME_RATE;

			// 速度から座標を移動
			cloth.particle[j][i].position.x += cloth.particle[j][i].velocity.x * Time::deltaTime;
			cloth.particle[j][i].position.y += cloth.particle[j][i].velocity.y * Time::deltaTime;
			cloth.particle[j][i].position.z += cloth.particle[j][i].velocity.z * Time::deltaTime;
		}
	}
}