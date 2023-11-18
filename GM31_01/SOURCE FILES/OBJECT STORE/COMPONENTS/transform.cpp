#include "component.h"

void Transform::Start()
{
	culling = true;

	drawRadius = 1.0f;

	Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	Quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);

	GlobalPosition = Position;
}

void Transform::End()
{

}

void Transform::Update()
{
	//D3DXQUATERNION quat;
	//D3DXQUATERNION result = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	//D3DXQuaternionRotationAxis(&quat, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), Rotation.x);
	//result *= quat;

	//D3DXQuaternionRotationYawPitchRoll(&Quaternion, Rotation.y, Rotation.x, Rotation.z);
}

void Transform::Draw()
{

}


void Transform::FaceTowards(GAMEOBJECT* object)
{
	//Y-Axis
	float X = object->transform->Position.x;
	float Z = object->transform->Position.z;

	float corrector = 180;

	float height = (Z - Position.z);
	float base = (X - Position.x);

	if (object->transform->Position.z < Position.z) { corrector = 180; }
	else { corrector = 0; }

	Rotation.y = (D3DXToDegree(atanf(base / height))) + corrector;
}

void Transform::FaceTowards(D3DXVECTOR3 value)
{
	float X = value.x;
	float Z = value.z;

	float corrector = 180;

	float height = (Z - Position.z);
	float base = (X - Position.x);

	if (value.z < 0) { corrector = 180; }
	else { corrector = 0; }

	Rotation.y = (D3DXToDegree(atanf(base / height))) + corrector;
}

void Transform::FaceTowardsXYZ(GAMEOBJECT* object)
{
	D3DXVECTOR3 X;
	D3DXVECTOR3 Y(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 Z = object->transform->Position;

	Z -= Position;
	D3DXVec3Normalize(&Z, &Z);

	D3DXVec3Cross(&X, &Y, &Z);
	D3DXVec3Normalize(&X, &X);

	D3DXVec3Cross(&Y, &Z, &X);
	D3DXVec3Normalize(&Y, &Y);

	D3DXMatrixIdentity(&gameObject->GetRotationMatrix());

	D3DXMATRIX RotationMatrix = gameObject->GetRotationMatrix();

	RotationMatrix._11 = X.x;
	RotationMatrix._12 = X.y;
	RotationMatrix._13 = X.z;

	RotationMatrix._21 = Y.x;
	RotationMatrix._22 = Y.y;
	RotationMatrix._23 = Y.z;

	RotationMatrix._31 = Z.x;
	RotationMatrix._32 = Z.y;
	RotationMatrix._33 = Z.z;

	gameObject->SetRotationMatrix(RotationMatrix);
}

float Transform::FaceInDirection()
{
	if (gameObject->rigidbody == nullptr)
	{
		return 0.0f;
	}

	float corrector = 180;
	float height;
	float base;

	//Y-Axis
	corrector = 180;
	height = gameObject->rigidbody->Speed.z;//	 (Z + gameObject->rigidbody->Speed.z);
	base = gameObject->rigidbody->Speed.x;//(X + gameObject->rigidbody->Speed.x);

	//height = Direction.z;
	//base = Direction.x;
	if (height < 0.0f) { corrector = 180; }
	else { corrector = 0; }

	return (D3DXToDegree(atanf(base / height))) + corrector;

	////X-Axis
	//corrector = 180;
	//height = gameObject->rigidbody->Speed.y;//(Z + gameObject->rigidbody->Speed.z);
	//base = gameObject->rigidbody->Speed.z;//(X + gameObject->rigidbody->Speed.x);

	//if (height < 0.0f) { corrector = 180; }
	//else { corrector = 0; }

	//transform->Rotation.x = (D3DXToDegree(atanf(base / height))) + corrector;

	////Z-Axis
	//corrector = 180;
	//height = gameObject->rigidbody->Speed.y;//(Z + gameObject->rigidbody->Speed.z);
	//base = gameObject->rigidbody->Speed.x;//(X + gameObject->rigidbody->Speed.x);

	//if (height < 0.0f) { corrector = 180; }
	//else { corrector = 0; }

	//transform->Rotation.z = (D3DXToDegree(atanf(base / height))) + corrector;
}
D3DXMATRIX Transform::FaceInDirectionXYZ()
{
	if (gameObject->rigidbody == nullptr)
	{
		return gameObject->GetRotationMatrix();
	}

	gameObject->SetFaceInDirection(true);

	D3DXVECTOR3 X;
	D3DXVECTOR3 Y(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 Z = gameObject->rigidbody->Speed;

	Z -= Position;
	D3DXVec3Normalize(&Z, &Z);

	D3DXVec3Cross(&X, &Y, &Z);
	D3DXVec3Normalize(&X, &X);

	D3DXVec3Cross(&Y, &Z, &X);
	D3DXVec3Normalize(&Y, &Y);

	D3DXMATRIX rot;

	rot._11 = X.x;
	rot._12 = X.y;
	rot._13 = X.z;

	rot._21 = Y.x;
	rot._22 = Y.y;
	rot._23 = Y.z;

	rot._31 = Z.x;
	rot._32 = Z.y;
	rot._33 = Z.z;

	return rot;
}

//D3DXVECTOR3 Transform::GetForwardDirection()
//{
//	D3DXMATRIX rot;
//	D3DXMatrixRotationYawPitchRoll(&rot, D3DXToRadian(Rotation.y), D3DXToRadian(Rotation.x), D3DXToRadian(Rotation.z));
//
//	D3DXVECTOR3 returner;
//	returner.x = rot._31;
//	returner.y = rot._32;
//	returner.z = rot._33;
//
//	return returner;
//}

D3DXVECTOR3 Transform::GetForwardDirection()
{
	D3DXMATRIX rot;
	D3DXMatrixRotationQuaternion(&rot, &Quaternion);

	D3DXVECTOR3 returner;
	returner.x = rot._31;
	returner.y = rot._32;
	returner.z = rot._33;

	return returner;
}

D3DXVECTOR3 Transform::GetRightDirection()
{
	D3DXMATRIX rot;
	D3DXMatrixRotationQuaternion(&rot, &Quaternion);

	D3DXVECTOR3 returner;
	returner.x = rot._11;
	returner.y = rot._12;
	returner.z = rot._13;

	return returner;
}

//D3DXVECTOR3 Transform::GetRightDirection()
//{
//	D3DXMATRIX rot;
//	D3DXMatrixRotationYawPitchRoll(&rot, D3DXToRadian(Rotation.y), D3DXToRadian(Rotation.x), D3DXToRadian(Rotation.z));
//
//	D3DXVECTOR3 returner;
//	returner.x = rot._11;
//	returner.y = rot._12;
//	returner.z = rot._13;
//
//	return returner;
//}

float Transform::DistanceFrom(GAMEOBJECT* from)
{
	D3DXVECTOR3 distance;
	distance = Position - from->transform->Position;
	float finalDistance = D3DXVec3Length(&distance);

	return  finalDistance;
}
float Transform::DistanceFromWithY0(GAMEOBJECT* from)
{
	D3DXVECTOR3 distance;
	distance = Position - from->transform->Position;
	distance.y = 0.0f;
	float finalDistance = D3DXVec3Length(&distance);

	return  finalDistance;
}