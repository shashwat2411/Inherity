#include "component.h"

void Rigidbody::Start()
{
	useGravity = false;

	Acceleration = ACCELERATION_VALUE;
	mass = 1.0f;
	distance = 0.0f;
	groundLevel = 0.0f;

	Speed = D3DXVECTOR3(0.00001f, 0.0f, 0.0f);

	gameObject->rigidbody = this;
}

void Rigidbody::End()
{

}

void Rigidbody::Update()
{
	if (FollowTarget != nullptr)
	{
		D3DXVECTOR3 diff = FollowTarget->transform->Position - gameObject->transform->Position;

		gameObject->transform->Position = (FollowTarget->transform->Position - (*D3DXVec3Normalize(&diff, &diff) * distance)) * Time::fixedTimeScale;

		gameObject->transform->FaceTowardsXYZ(FollowTarget);
	}
	else
	{
	}

	if (useGravity == true)
	{
		gameObject->rigidbody->Speed.y -= (GRAVITY_CONSTANT * Time::deltaTime) * GRAVITY_ACCELERATION * mass;

		if (gameObject->transform->Position.y <= groundLevel)
		{
			gameObject->transform->Position.y = groundLevel;
			gameObject->rigidbody->Speed.y = 0.0f;
		}
	}
}

void Rigidbody::Draw()
{

}

void Rigidbody::EngineDisplay()
{
	if (ImGui::TreeNode("Rigidbody"))
	{
		ImGui::Checkbox("Use Gravity", &useGravity);

		ImGui::DragFloat("Acceleration", &Acceleration);
		ImGui::DragFloat3("Speed", Speed);

		ImGui::TreePop();
		ImGui::Spacing();
	}
}
