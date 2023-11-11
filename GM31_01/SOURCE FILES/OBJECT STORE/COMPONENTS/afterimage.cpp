#include "../OBJECT STORE/component.h"

void Afterimage::Start()
{
	//if (gameObject->GetComponent<Rigidbody>() == nullptr && gameObject->Parent->GetComponent<Rigidbody>() == nullptr)
	{
		//gameObject->RemoveComponent<Afterimage>();
		//return;
	}

	for (int i = 0; i < MAX_AFTERIMAGES; i++) { D3DXMatrixIdentity(&gameObject->GetWorldMatrix(i)); }

	D3DXMatrixIdentity(&gameObject->GetRotationMatrix());
	D3DXMatrixIdentity(&gameObject->GetTransformMatrix());
	D3DXMatrixIdentity(&gameObject->GetScaleMatrix());

	D3DXVECTOR3 Position = gameObject->transform->Position;
	D3DXVECTOR3 Rotation = gameObject->transform->Rotation;
	D3DXVECTOR3 Scale = gameObject->transform->Scale;

	D3DXMatrixScaling(&gameObject->GetScaleMatrix(), Scale.x, Scale.y, Scale.z); \
		D3DXMatrixRotationYawPitchRoll(&gameObject->GetRotationMatrix(), Rotation.y, Rotation.x, Rotation.z); \
		D3DXMatrixTranslation(&gameObject->GetTransformMatrix(), Position.x, Position.y, Position.z);

	D3DXMatrixMultiply(&gameObject->GetWorldMatrix(), &gameObject->GetScaleMatrix(), &gameObject->GetRotationMatrix());  //World = Scaling * Rotation
	D3DXMatrixMultiply(&gameObject->GetWorldMatrix(), &gameObject->GetWorldMatrix(), &gameObject->GetTransformMatrix());  //World = World * Translation

	for (int i = 1; i < MAX_AFTERIMAGES; i++) { gameObject->SetWorldMatrix(gameObject->GetWorldMatrix(), i); }	//リングバッファの初期化
	gameObject->SetRingCounter(0);
}

void Afterimage::Update()
{
	//Rigidbody* updater = (gameObject->rigidbody == nullptr ? gameObject->Parent->rigidbody : gameObject->rigidbody);
	//if (fabs(updater->Speed.x) > 0.02f || fabs(updater->Speed.y) > 0.02f || fabs(updater->Speed.z) > 0.02f)
	{
		gameObject->SetRingCounter(gameObject->GetRingCounter() + 1);
		gameObject->SetRingCounter(gameObject->GetRingCounter() % MAX_AFTERIMAGES);
	}
}