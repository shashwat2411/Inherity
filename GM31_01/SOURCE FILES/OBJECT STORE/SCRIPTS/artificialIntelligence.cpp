#include "script.h"
#include "manager.h"

void ArtificialIntelligence::Start()
{
	flip = false;

	index = 0;
	nextIndex = index + 1;

	distance = 20.0f;
	timerVector["time"] = 0.0f;
	timerVector["speed"] = 0.001f;
	timerVector["followSpeed"] = 0.05f;

	startPosition = gameObject->transform->Position;
	returnPosition = startPosition;

	state = ROAM;

	SCENE* scene = Manager::GetScene();

	for (int i = 0; i < 4; i++)
	{
		std::string str = "Point " + std::to_string(i);
		EMPTYOBJECT* point = scene->AddGameObject<EMPTYOBJECT>(str.c_str(), GIZMO_LAYER);
		point->transform->Scale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);

#ifdef DEBUG
		point->AddComponent<MeshFilter>()->SetModel(ModelReader::SPHERE_COLLIDER_M);
#endif
		points.push_back(point);
	}

	points[0]->transform->Position = startPosition;
	points[1]->transform->Position = startPosition + gameObject->transform->GetForwardDirection() * distance;
	points[2]->transform->Position = points[1]->transform->Position - (flip ? -1 : 1) * gameObject->transform->GetRightDirection() * distance;
	points[3]->transform->Position = startPosition - (flip ? -1 : 1) * gameObject->transform->GetRightDirection() * distance;

	projector = gameObject->AddComponent<Projector>();
	projector->SetTargetName("Player");
}

void ArtificialIntelligence::End()
{

}

void ArtificialIntelligence::Update()
{
	if (projector->GetTarget() != nullptr) { state = FOLLOW; }

	switch (state)
	{
	case ROAM:
		Roam();
		break;

	case FOLLOW:
		Follow();
		break;

	case RETURN:
		Return();
		break;

	default:
		break;
	}

}

void ArtificialIntelligence::Draw()
{
	if (DebugManager::play == false || DebugManager::paused == true)
	{
		startPosition = gameObject->transform->Position;

		points[0]->transform->Position = startPosition;
		points[1]->transform->Position = startPosition + gameObject->transform->GetForwardDirection() * distance;
		points[2]->transform->Position = points[1]->transform->Position - (flip ? -1 : 1) * gameObject->transform->GetRightDirection() * distance;
		points[3]->transform->Position = startPosition - (flip ? -1 : 1) * gameObject->transform->GetRightDirection() * distance;
	}
}

const char* enemyStatus[ArtificialIntelligence::ENEMY_STATE_MAX] =
{
	"ROAM",
	"FOLLOW",
	"RETURN"
};

void ArtificialIntelligence::EngineDisplay()
{
	if (ImGui::TreeNode("Enemy AI"))
	{
		int stat = (int)state;
		ImGui::PushItemWidth(-FLT_MIN);
		ImGui::SliderInt(" ", &stat, 0, ENEMY_STATE_MAX - 1, enemyStatus[stat]);

		DebugManager::FloatDisplay(&distance, -FLT_MIN, "Distance", true, D3DXVECTOR2(0.01f, 0.0f), 1);
		DebugManager::FloatDisplay(&timerVector["speed"], -FLT_MIN, "Speed", true, D3DXVECTOR2(0.001f, 0.0f), 2);
		DebugManager::FloatDisplay(&timerVector["followSpeed"], -FLT_MIN, "Follow Speed", true, D3DXVECTOR2(0.001f, 0.0f), 3);
		DebugManager::BoolDisplay(&flip, -180.0f, "Flip", 4);

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

void ArtificialIntelligence::Roam()
{
	timerVector["time"] += timerVector["speed"] * Time::fixedTimeScale;

	if (nextIndex < 4)
	{
		gameObject->transform->FaceTowards(points[nextIndex]);
		gameObject->transform->Position = Vector3::Lerp(points[index]->transform->Position, points[nextIndex]->transform->Position, timerVector["time"]);
	}

	if (timerVector["time"] >= 1.0f)
	{
		timerVector["time"] = 0.0f;
		index = ((index + 1) < 4 ? (index + 1) : 0);
		nextIndex = ((nextIndex + 1) < 4 ? (nextIndex + 1) : 0);
	}
}

void ArtificialIntelligence::Follow()
{
	if (projector->GetTarget() == nullptr) 
	{ 
		state = RETURN;
		returnPosition = Vector3::Lerp(points[index]->transform->Position, points[nextIndex]->transform->Position, timerVector["time"]);
		return; 
	}

	D3DXVECTOR3 direction = gameObject->transform->GetForwardDirection();
	direction *= timerVector["followSpeed"];
	direction.y = 0.0f;

	gameObject->rigidbody->Speed = direction;
}

void ArtificialIntelligence::Return()
{
	D3DXVECTOR3 direction = returnPosition - gameObject->transform->Position;
	D3DXVec3Normalize(&direction, &direction);
	direction.y = 0.0f;

	direction *= timerVector["followSpeed"];

	gameObject->rigidbody->Speed = direction;

	gameObject->transform->Rotation.y = gameObject->transform->FaceInDirection();
	if (gameObject->transform->DistanceFrom(returnPosition) < 0.1f) { state = ROAM; }
}