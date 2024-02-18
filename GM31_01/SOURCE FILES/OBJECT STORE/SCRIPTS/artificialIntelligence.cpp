#include "script.h"
#include "manager.h"

void ArtificialIntelligence::Start()
{
	flip = false;
	lock = true;
	shot = false;

	index = 0;
	nextIndex = index + 1;
	danceIndex = -1;

	distance = 20.0f;
	timerVector["time"] = 0.0f;
	timerVector["speed"] = 0.001f;
	timerVector["followSpeed"] = 0.08f;
	timerVector["maxDistance"] = 20.0f;
	timerVector["timer"] = 0.0f;

	timerVector["followDistance"] = 3.0f;
	timerVector["waitMaxTime"] = 0.4f;

	timerVector["attackMaxTime"] = 3.0f;
	timerVector["attackSpeed"] = 1.0f;

	timerVector["deathTimer"] = 0.0f;

	timerVector["findMaxTime"] = 2.0f;
	timerVector["rotationSpeed"] = 0.03f;
	timerVector["rotationTimer"] = 0.0f;
	timerVector["findValue"] = 45.0f;
	timerVector["rotationStart"] = 0.0f;
	timerVector["findRotation"] = timerVector["rotationStart"] + timerVector["power"] * timerVector["findValue"];
	timerVector["power"] = 1.0f;

	startPosition = gameObject->transform->Position;
	returnPosition = startPosition;

	state = ROAM;

	target = nullptr;

	targetName = "Damager";

	SCENE* scene = Manager::GetScene();

	for (int i = 0; i < 4; i++)
	{
		std::string str = "Point " + std::to_string(i + 1);
		EMPTYOBJECT* point = scene->AddGameObject<EMPTYOBJECT>(str.c_str(), GIZMO_LAYER);
		point->transform->Scale = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
		
#ifdef DEBUG
		point->AddComponent<MeshFilter>()->SetModel(ModelReader::SPHERE_COLLIDER_M);
		point->SetDepthShadow(false);
#endif
		points.push_back(point);
	}

	points[0]->transform->Position = startPosition;
	points[1]->transform->Position = startPosition + gameObject->transform->GetForwardDirection() * distance;
	points[2]->transform->Position = points[1]->transform->Position - (flip ? -1 : 1) * gameObject->transform->GetRightDirection() * distance;
	points[3]->transform->Position = startPosition - (flip ? -1 : 1) * gameObject->transform->GetRightDirection() * distance;

	seeker = Manager::GetScene()->Find(targetName.c_str());

	model = gameObject->GetChildren()[0]->GetComponent<MeshFilter>();


#ifdef DEBUG
	//attackDistance = scene->AddGameObject<EMPTYOBJECT>("Attack Distance", GIZMO_LAYER);
	//attackDistance->SetSave(false);
	//attackDistance->AddComponent<MeshFilter>()->SetModel(ModelReader::SPHERE_COLLIDER_M);
	//attackDistance->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	//attackDistance->SetDepthShadow(false);
#endif
}

void ArtificialIntelligence::End()
{
	
}

void ArtificialIntelligence::Update()
{
	//gameObject->rigidbody->groundLevel = Manager::GetScene()->FindGameObject<PLANE>()->GetComponent<MeshField>()->GetHeight(gameObject->transform->Position);

	switch (state)
	{
	case ROAM:
		model->SetAnimationBlend("Enemy_Walk", true);
		Roam();
		break;

	case FOLLOW:
		model->SetAnimationBlend("Enemy_Run", true);
		Follow();
		break;

	case WAIT:
		model->SetAnimationBlend("Enemy_Wait", true);
		Wait();
		break;

	case ATTACK:
		Attack();
		break;

	case FIND:
		model->SetAnimationBlend("Enemy_Find", true);
		Find();
		break;

	case RETURN:
		model->SetAnimationBlend("Enemy_Walk", true);
		Return();
		break;

	case DEATH:
		Death();
		break;

	case VICTORY:
		Victory();
		break;

	default:
		break;
	}

}

void ArtificialIntelligence::Draw()
{
	//attackDistance->transform->Position = gameObject->transform->Position;
	//attackDistance->transform->Scale = D3DXVECTOR3(timerVector["followDistance"], timerVector["followDistance"], timerVector["followDistance"]);

	if (DebugManager::play == false || DebugManager::paused == true)
	{
		startPosition = gameObject->transform->Position;

		points[0]->transform->Position = startPosition;

		if (lock == true)
		{
			points[1]->transform->Position = startPosition + gameObject->transform->GetForwardDirection() * distance;
			points[2]->transform->Position = points[1]->transform->Position - (flip ? -1 : 1) * gameObject->transform->GetRightDirection() * distance;
			points[3]->transform->Position = startPosition - (flip ? -1 : 1) * gameObject->transform->GetRightDirection() * distance;
		}
	}
}

const char* enemyStatus[ArtificialIntelligence::ENEMY_STATE_MAX] =
{
	"ROAM",
	"FOLLOW",
	"WAIT",
	"ATTACK",
	"FIND",
	"RETURN",
	"DEATH",
	"DANCE"
};

void ArtificialIntelligence::EngineDisplay()
{
	if (ImGui::TreeNode("Enemy AI"))
	{
		int stat = (int)state;
		ImGui::PushItemWidth(-FLT_MIN);
		ImGui::SliderInt(" ", &stat, 0, ENEMY_STATE_MAX - 1, enemyStatus[stat]);

		ImGui::Text("\n");

		DebugManager::FloatDisplay(&distance, -FLT_MIN, "Distance", true, D3DXVECTOR2(0.01f, 0.0f), 1);
		DebugManager::FloatDisplay(&timerVector["speed"], -FLT_MIN, "Speed", true, D3DXVECTOR2(0.001f, 0.0f), 2);
		DebugManager::FloatDisplay(&timerVector["followSpeed"], -FLT_MIN, "Follow Speed", true, D3DXVECTOR2(0.001f, 0.0f), 3);
		DebugManager::BoolDisplay(&flip, -180.0f, "Flip", 4); ImGui::SameLine();
		DebugManager::BoolDisplay(&lock, -70.0f, "Lock", 5);

		ImGui::Text("\n");
		
		ImGui::Text("Seeker : %s", (seeker ? seeker->GetTag().c_str() : "nullptr"));
		ImGui::Text("Target : %s", (target ? target->GetTag().c_str() : "nullptr"));
		ImGui::Text("Target Name : %s", targetName.c_str());

		ImGui::Text("\n");
		ImGui::Text("\n");

		DebugManager::FloatDisplay(&timerVector["followDistance"], -FLT_MIN, "Follow Distance", true, D3DXVECTOR2(0.01f, 0.0f), 8);
		DebugManager::FloatDisplay(&timerVector["waitMaxTime"], -FLT_MIN, "Wait Max Time", true, D3DXVECTOR2(0.01f, 0.0f), 9);

		ImGui::Text("\n");

		DebugManager::FloatDisplay(&timerVector["attackMaxTime"], -FLT_MIN, "Attack Max Time", true, D3DXVECTOR2(0.01f, 0.0f), 10);
		DebugManager::FloatDisplay(&timerVector["attackSpeed"], -FLT_MIN, "Attack Speed", true, D3DXVECTOR2(0.01f, 0.0f), 11);

		ImGui::Text("\n");

		DebugManager::FloatDisplay(&timerVector["findMaxTime"], -FLT_MIN, "Find Max Time", true, D3DXVECTOR2(0.01f, 0.0f), 12);
		DebugManager::FloatDisplay(&timerVector["findValue"], -FLT_MIN, "Find Value", true, D3DXVECTOR2(0.01f, 0.0f), 13);
		DebugManager::FloatDisplay(&timerVector["rotationSpeed"], -FLT_MIN, "Rotation Speed", true, D3DXVECTOR2(0.01f, 0.0f), 14);

		ImGui::Text("\n");

		DebugManager::FloatDisplay(&timerVector["timer"], -FLT_MIN, "Timer", true, D3DXVECTOR2(0.01f, 0.0f), 15, true);

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

void ArtificialIntelligence::OnCollisionEnter(GAMEOBJECT* obj)
{
	if (seeker != nullptr && (state == ROAM || state == RETURN || state == FIND))
	{
		if (obj->GetTag() == seeker->GetTag() || obj->GetTag() == "Damager")
		{
			target = obj;
			state = FOLLOW;
		}
	}
}

void ArtificialIntelligence::Roam()
{
	Finder();

	timerVector["time"] += timerVector["speed"] * Time::fixedTimeScale;

	if (nextIndex < 4)
	{
		gameObject->transform->FaceTowards(points[nextIndex]);
		D3DXVECTOR3 displacement = Vector3::Lerp(points[index]->transform->Position, points[nextIndex]->transform->Position, timerVector["time"]);

		gameObject->transform->Position.x = displacement.x;
		gameObject->transform->Position.z = displacement.z;
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
	Finder();

	if (target == nullptr)
	{
		state = FIND;
		model->SetAnimationBlend("Enemy_Find");
		returnPosition = Vector3::Lerp(points[index]->transform->Position, points[nextIndex]->transform->Position, timerVector["time"]);
		returnPosition.y = gameObject->transform->Position.y;

		return;
	}

	D3DXVECTOR3 direction = target->transform->GlobalPosition - gameObject->transform->GlobalPosition;
	D3DXVec3Normalize(&direction, &direction);
	direction *= timerVector["followSpeed"];
	direction.y = 0.0f;

	gameObject->rigidbody->Speed = direction;
	gameObject->transform->Rotation.y = gameObject->transform->FaceInDirection();

	if (gameObject->transform->DistanceFrom(target) < timerVector["followDistance"])
	{
		state = WAIT;
		timerVector["timer"] = 0.0f;
	}
}

void ArtificialIntelligence::Return()
{
	Finder();

	D3DXVECTOR3 direction = returnPosition - gameObject->transform->GlobalPosition;
	D3DXVec3Normalize(&direction, &direction);
	direction.y = 0.0f;

	direction *= timerVector["followSpeed"];

	gameObject->rigidbody->Speed = direction;

	gameObject->transform->Rotation.y = gameObject->transform->FaceInDirection();
	if (gameObject->transform->DistanceFrom(returnPosition) < timerVector["followSpeed"]) 
	{ 
		state = ROAM;
	}
}

void ArtificialIntelligence::Wait()
{
	timerVector["timer"] += (1.0f / FRAME_RATE) * Time::fixedTimeScale;
	if (timerVector["timer"] >= timerVector["waitMaxTime"])
	{
		timerVector["timer"] = 0.0f;
		state = ATTACK;
		model->SetAnimationBlend("Enemy_Attack_1");
		SoundReader::GetReadSound(SoundReader::SWING)->Play(false, 0.3f);
		//D3DXVECTOR3 direction = target->transform->GlobalPosition - gameObject->transform->GlobalPosition;
		//D3DXVec3Normalize(&direction, &direction);
		//direction *= timerVector["attackSpeed"];
		//direction.y = 0.0f;
		//
		//gameObject->rigidbody->Speed = direction;
		//gameObject->transform->Rotation.y = gameObject->transform->FaceInDirection();
	}

}

void ArtificialIntelligence::Attack()
{
	if (model->GetAnimationOver("Enemy_Attack_1") == true)
	{
		if (seeker != nullptr)
		{
			D3DXVECTOR3 v, u;
			v = gameObject->transform->GetForwardDirection();
			u = gameObject->transform->GlobalPosition - seeker->transform->GlobalPosition;

			if (D3DXVec3Dot(&u, &v) < 0.0f && Vector3::Magnitude(u) < timerVector["maxDistance"])
			{
				if (gameObject->transform->DistanceFrom(target) < timerVector["followDistance"])
				{
					model->SetAnimationBlend("Enemy_Attack_1");
					return;
				}
			}

		}

		state = FOLLOW;
	}

	/*
	timerVector["timer"] += (1.0f / FRAME_RATE) * Time::fixedTimeScale;
	if (timerVector["timer"] >= timerVector["attackMaxTime"])
	{
		Finder();

		if (target == nullptr)
		{
			state = FIND;
			timerVector["rotationStart"] = gameObject->transform->Rotation.y;
			timerVector["findRotation"] = timerVector["rotationStart"] + timerVector["findValue"];
			return;
		}

		state = FOLLOW;
	}
	*/
}

void ArtificialIntelligence::Find()
{
	Finder();

	if (model->GetAnimationOver("Enemy_Find") == true)
	{
		state = RETURN;
	}

	//timerVector["rotationTimer"] += timerVector["rotationSpeed"] * Time::fixedTimeScale;


	//gameObject->transform->Rotation.y = Mathf::Lerp(gameObject->transform->Rotation.y, timerVector["findRotation"], timerVector["rotationTimer"]);
	//if (timerVector["rotationTimer"] >= 1.0f)
	//{
	//	timerVector["power"] *= -1.0f;
	//	timerVector["rotationTimer"] = 0.0f;
	//	timerVector["findRotation"] = timerVector["rotationStart"] + timerVector["power"] * timerVector["findValue"];
	//}

	//timerVector["timer"] += (1.0f / FRAME_RATE) * Time::fixedTimeScale;
	//if (timerVector["timer"] >= timerVector["findMaxTime"])
	//{
	//	state = FOLLOW;
	//}
}

void ArtificialIntelligence::Death()
{
	model->SetAnimationBlend("Enemy_Death", false, 0.4f);

	timerVector["deathTimer"] += Time::deltaTime;

	if (timerVector["deathTimer"] >= 3.58f)
	{
		if (model->GetStop() == false)
		{
			model->SetStop(true);
			model->gameObject->GetMaterial()->SetFloat("_Dissolve", 1.0f);
		}
		else
		{
			float temp = model->gameObject->GetMaterial()->GetFloat("_Threshold");
			temp = Mathf::Lerp(temp, 0.0f, 0.04f);
			model->gameObject->GetMaterial()->SetFloat("_Threshold", temp);

			if (temp <= 0.01f)
			{
				gameObject->Destroy();
			}
		}
	}
}

void ArtificialIntelligence::Victory()
{
	if (danceIndex == 0) { model->SetAnimationBlend("Enemy_Dance_1", true); }
	else { model->SetAnimationBlend("Enemy_Dance_2", true); }
}

void ArtificialIntelligence::Finder()
{
	if (shot == false)
	{
		if (seeker != nullptr)
		{
			D3DXVECTOR3 v, u;
			v = gameObject->transform->GetForwardDirection();
			u = gameObject->transform->GlobalPosition - seeker->transform->GlobalPosition;

			SphereCollider* collider = gameObject->GetComponent<SphereCollider>();

			if (D3DXVec3Dot(&u, &v) < 0.0f && Vector3::Magnitude(u) < timerVector["maxDistance"])
			{
				target = seeker;
				state = FOLLOW;
			}
			else if (collider)
			{
				if (gameObject->GetComponent<SphereCollider>()->GetCollision() == false)
				{
					target = nullptr;
				}
			}
		}
		else
		{
			seeker = Manager::GetScene()->Find(targetName.c_str());
		}
	}
}

void ArtificialIntelligence::Dancing()
{
	state = VICTORY;

	if (danceIndex < 0)
	{
		danceIndex = rand() % 2;
	}
}

void ArtificialIntelligence::SetStateToReturn()
{
	target = nullptr;
	state = RETURN;
}

void ArtificialIntelligence::SetStateToFollow()
{
	if (seeker != nullptr && (state == ROAM || state == RETURN || state == FIND))
	{
		target = seeker;
		state = FOLLOW;
		shot = true;
	}
}