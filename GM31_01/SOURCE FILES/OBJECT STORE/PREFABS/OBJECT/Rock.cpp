#include "prefab.h"

void ROCK::Start()
{
	Init();

	SetTag("Rock");

	AddComponent<MeshFilter>()->SetModel(ModelReader::ROCK_M);
	AddMaterial<DissolveMaterial>();

	dissolveSpeed = 0.0075f;
	dissolveThreshold = 0.0f;
	GetMaterial()->SetFloat("_Threshold", dissolveThreshold);

	transform->Position = D3DXVECTOR3(-4.0f, 1.0f, 0.0f);
	transform->Scale = D3DXVECTOR3(5.0f, 5.0f, 5.0f);
}

void ROCK::Update()
{
	if (dissolveThreshold > 1.1f || dissolveThreshold < 0.0f) { dissolveSpeed *= -1.0f; }
	dissolveThreshold += dissolveSpeed * Time::fixedTimeScale;

	GetMaterial()->SetFloat("_Threshold", dissolveThreshold);
}