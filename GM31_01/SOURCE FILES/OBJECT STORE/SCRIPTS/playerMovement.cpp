#include "script.h"
#include "manager.h"
#include "input.h"

float angle = 0.0f;
float dangle = 0.0f;
bool animationShift = false;

D3DXVECTOR3 rotationDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

void PlayerMovement::Start()
{
	aim = false;
	move = false;
	diagonal = false;
	gunSelection = false;
	setAnimation = false;
	invincibility = false;

	idleCounter = 0;
	timerVector["deathTimer"] = 0.0f;
	timerVector["hitTimer"] = 0.0f;
	timerVector["rollSpeed"] = 0.4f;
	timerVector["dissolveSpeedAppear"] = 0.05f;
	timerVector["dissolveSpeedDissappear"] = 0.1f;

	timerVector["shootCountdown"] = 0.0f;
	timerVector["shootCooldown"] = 0.0333f;

	timerVector["invincibilityCounter"] = 0.0f;

	rotationDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	playerState = NORMAL_MOVE_PS;

	gameObject->AddComponent<Rigidbody>()->useGravity = true;
	gameObject->rigidbody->groundLevel = 1.01f;
	gameObject->rigidbody->FollowTarget = nullptr;

	
}

void PlayerMovement::End()
{
}

void PlayerMovement::Update()
{
	//Trash
	/*
	|----------------------------------TRASH--------------------------------
	|gameObject->transform->Rotation.z += 1.0f;
	|
	|OpenWorldControls
	|{
	|	if (Input::GetKeyPress('W'))
	|	{
	|		directionZ = gameObject->GetForwardDirection();
	|	}
	|	else if (Input::GetKeyPress('S'))
	|	{
	|		directionZ = -gameObject->GetForwardDirection();
	|	}
	|	if (Input::GetKeyPress('A'))
	|	{
	|		directionX = -gameObject->GetRightDirection();
	|	}
	|	else if (Input::GetKeyPress('D'))
	|	{
	|		directionX = gameObject->GetRightDirection();
	|	}
	|}
	|gameObject->rigidbody->Speed += -gameObject->GetForwardDirection() * SPEED_VALUE * ACCELERATION_VALUE;
	|
	|if (Input::GetKeyPress('A'))
	|{
	|	gameObject->rigidbody->Speed.x -= SPEED_VALUE * ACCELERATION_VALUE;
	|}
	|else if (Input::GetKeyPress('D'))
	|{
	|	gameObject->rigidbody->Speed.x += SPEED_VALUE * ACCELERATION_VALUE;
	|}
	|if (Input::GetKeyPress('W'))
	|{
	|	gameObject->rigidbody->Speed.z += SPEED_VALUE * ACCELERATION_VALUE;
	|}
	|else if (Input::GetKeyPress('S'))
	|{
	|	gameObject->rigidbody->Speed.z -= SPEED_VALUE * ACCELERATION_VALUE;
	|}
	|
	|if (Input::GetKeyPress('Q'))
	|{
	|	gameObject->transform->Rotation.y -= 1.0f;
	|}
	|else if (Input::GetKeyPress('E'))
	|{
	|	gameObject->transform->Rotation.y += 1.0f;
	|}
	|
	|if (Input::GetKeyTrigger(VK_SPACE))
	|{
	|	//gameObject->rigidbody->Speed.y = 0.3f;
	|	Animator* animator = gameObject->GetComponentInChildren<Animator>();
	|	Animation::ANIMATION_STATUS stat1 = animator->GetAnimationState(0);
	|	Animation::ANIMATION_STATUS stat2 = animator->GetAnimationState(2);
	|	if ((stat1 == Animation::STANDBY || stat1 == Animation::END) && (stat2 == Animation::STANDBY || stat2 == Animation::END))
	|	{
	|		if (animator->GetAnimationState(1) == Animation::PLAYBACK)
	|		{
	|			animator->StopAnimation(1);
	|			animator->PlayAnimation(4);
	|		}
	|		else
	|		{
	|			animator->PlayAnimation(3);
	|		}
	|	}
	|}
	|-----------------------------------------------------------------------
	*/

	gun1->transform->Rotation.z = 47.5f;
	gun2->transform->Rotation.z = 45.5f;

	//Direction
	{
		D3DXVECTOR3 directionZ(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 directionX(0.0f, 0.0f, 0.0f);

		float vertical = Input::Vertical();
		float horizontal = Input::Horizontal();

		directionZ = camera->GetForward() * vertical;
		directionX = camera->GetRight() * horizontal;

		directionZ.y = 0.0f;
		directionX.y = 0.0f;
		D3DXVec3Normalize(&directionZ, &directionZ);
		D3DXVec3Normalize(&directionX, &directionX);

		direction = directionX + directionZ;
		D3DXVec3Normalize(&direction, &direction);
	}

	if (playerState != ROLL_PS && playerState != DEATH_PS && playerState != HIT_PS)
	{
		if ((/*ImGui::IsKeyDown((ImGuiKey)656)*/ /*IsMouseRightPressed()*/  Input::GetButtonPress(AIM_KEYMAP)) && aim == false)
		{
			aim = true;
			playerState = AIMING_MOVE_PS;
			cameraController->SetBackUpDistance(D3DXVECTOR3(cameraController->GetBackUpDistance().x, 2.8f, 3.9f));
			//cameraController->SetFollowSpeed(D3DXVECTOR3(0.6f, 0.1f, 0.0f));
			cameraController->SetScreenLimit(D3DXVECTOR3(180.0f, 180.0f, -40.0f));
		}
		else if ((/*!ImGui::IsKeyDown((ImGuiKey)656)*/ /*!IsMouseRightPressed()*/  !Input::GetButtonPress(AIM_KEYMAP)) && aim == true)
		{
			aim = false;
			playerState = NORMAL_MOVE_PS;
			cameraController->SetBackUpDistance(D3DXVECTOR3(cameraController->GetBackUpDistance().x, 4.0f, 8.0f));
			//cameraController->SetFollowSpeed(D3DXVECTOR3(0.012f, 0.08f, 0.0f));
			cameraController->SetScreenLimit(D3DXVECTOR3(300.0f, 180.0f, -40.0f));
		}

		if (Input::GetButtonTrigger(ROLL_KEYMAP))
		{
			aim = false;
			playerState = ROLL_PS;
			model->SetAnimationBlend("Roll");

			gameObject->rigidbody->Speed = direction * timerVector["rollSpeed"];
			rotationDirection = direction;

			timerVector["invincibilityCounter"] = 0.0f;
		}
	}

	switch (playerState)
	{
	case NORMAL_MOVE_PS:
		NormalMove();
		break;

	case AIMING_MOVE_PS:
		AimingMove();
		break;

	case ROLL_PS:
		Roll();
		break;

	case DEATH_PS:
		Death();
		break;

	case HIT_PS:
		Hit();
		break;

	default:
		break;
	}

	if (playerState != DEATH_PS && playerState != HIT_PS)
	{
		gameObject->transform->Position += gameObject->rigidbody->Speed * Time::fixedTimeScale;

		gameObject->rigidbody->Speed.x *= 0.9f;
		gameObject->rigidbody->Speed.z *= 0.9f;

		angle = atan2f(rotationDirection.x, rotationDirection.z);

		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), angle);
		D3DXQuaternionSlerp(&model->gameObject->transform->Quaternion, &model->gameObject->transform->Quaternion, &quat, 0.2f * Time::fixedTimeScale);
	}

}

void PlayerMovement::Draw()
{

}

const char* playerStatus[PlayerMovement::PS_MAX] =
{
	"NORMAL_MOVE_PS",
	"AIMING_MOVE_PS",
	"ROLL_PS",
	"DEATH_PS",
	"HIT_PS"
};
int playerstat = 0;

void PlayerMovement::EngineDisplay()
{
	playerstat = (int)playerState;
	if (ImGui::TreeNode("Player Movement"))
	{
		ImGui::PushItemWidth(-FLT_MIN);
		ImGui::SliderInt(" \n", &playerstat, 0, PS_MAX - 1, playerStatus[playerstat]);

		DebugManager::BoolDisplay(&move, -146.0f, "Move", 1, true);

		DebugManager::FloatDisplay(&timerVector["rollSpeed"], -FLT_MIN, "Roll Speed", true, D3DXVECTOR2(0.01f, 0.0f), 2);
		DebugManager::FloatDisplay(&timerVector["dissolveSpeedAppear"], -FLT_MIN, "Dissolve Speed Appear", true, D3DXVECTOR2(0.01f, 0.0f), 3);
		DebugManager::FloatDisplay(&timerVector["dissolveSpeedDissappear"], -FLT_MIN, "Dissolve Speed Dissappear", true, D3DXVECTOR2(0.01f, 0.0f), 4);
		DebugManager::FloatDisplay(&timerVector["shootCooldown"], -FLT_MIN, "Shoot Cooldown", true, D3DXVECTOR2(0.01f, 0.0f), 5);

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

void PlayerMovement::NormalMove()
{
	float lerper = Mathf::Lerp(cameraController->GetFollowSpeed().x, 0.012f, 0.3f);
	cameraController->SetFollowSpeed(D3DXVECTOR3(lerper, cameraController->GetFollowSpeed().y, cameraController->GetFollowSpeed().z));
	//gameObject->rigidbody->groundLevel = Manager::GetScene()->FindGameObject<PLANE>()->GetComponent<MeshField>()->GetHeight(gameObject->transform->Position);

	float thres;
	thres = gun1->GetMaterial()->GetFloat("_Threshold");
	thres = Mathf::Lerp(thres, 0.0f, timerVector["dissolveSpeedDissappear"] * Time::fixedTimeScale);
	gun1->GetMaterial()->SetFloat("_Threshold", thres);

	thres = gun2->GetMaterial()->GetFloat("_Threshold");
	thres = Mathf::Lerp(thres, 0.0f, timerVector["dissolveSpeedDissappear"] * Time::fixedTimeScale);
	gun2->GetMaterial()->SetFloat("_Threshold", thres);

	//D3DXVECTOR3 directionZ(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 directionX(0.0f, 0.0f, 0.0f);

	//Camera* camera = Manager::GetScene()->GetCamera()->camera;

	if (fabs(Input::Vertical()) > 0.0f || fabs(Input::Horizontal()) > 0.0f)
	{
		//directionZ = camera->GetForward() * vertical;
		//directionX = camera->GetRight() * horizontal;

		move = true;
	}
	else
	{
		move = false;
	}

	{
		//if		(Input::GetButtonPress(FORWARD_KEYMAP))	{ /*directionZ = gameObject->transform->GetForwardDirection();	*/	directionZ =  camera->GetForward();	move = true; /*rotationDirection.y = 0.0f;	*/	}
		//else if (Input::GetButtonPress(BACK_KEYMAP))	{ /*directionZ = -gameObject->transform->GetForwardDirection();	*/	directionZ = -camera->GetForward();	move = true; /*rotationDirection.y = 180.0f;*/	}
		//if		(Input::GetButtonPress(LEFT_KEYMAP))	{ /*directionX = -gameObject->transform->GetRightDirection();	*/	directionX = -camera->GetRight();	move = true; /*rotationDirection.y = 270.0f;*/	}
		//else if (Input::GetButtonPress(RIGHT_KEYMAP))	{ /*directionX = gameObject->transform->GetRightDirection();	*/	directionX =  camera->GetRight();	move = true; /*rotationDirection.y = 90.0f;	*/	}

		//if		(Input::GetButtonPress(FORWARD_KEYMAP) && Input::GetButtonPress(LEFT_KEYMAP))	{ move = true; /*rotationDirection.y = 315.0f;*/	}
		//else if (Input::GetButtonPress(FORWARD_KEYMAP) && Input::GetButtonPress(RIGHT_KEYMAP))	{ move = true; /*rotationDirection.y = 45.0f; */	}
		//if		(Input::GetButtonPress(BACK_KEYMAP) && Input::GetButtonPress(LEFT_KEYMAP))		{ move = true; /*rotationDirection.y = 225.0f;*/	}
		//else if (Input::GetButtonPress(BACK_KEYMAP) && Input::GetButtonPress(RIGHT_KEYMAP))		{ move = true; /*rotationDirection.y = 135.0f;*/	}

		//if (!Input::GetButtonPress(FORWARD_KEYMAP) && !Input::GetButtonPress(LEFT_KEYMAP) && !Input::GetButtonPress(BACK_KEYMAP) && !Input::GetButtonPress(RIGHT_KEYMAP)) { move = false; }

		//directionZ.y = 0.0f;
		//directionX.y = 0.0f;
		//D3DXVec3Normalize(&directionZ, &directionZ);
		//D3DXVec3Normalize(&directionX, &directionX);

		//D3DXVECTOR3 direction = directionX + directionZ;
		//D3DXVec3Normalize(&direction, &direction);
	}

	D3DXVECTOR3 finalSpeed;
	finalSpeed.x = direction.x * SPEED_VALUE * gameObject->rigidbody->Acceleration;
	finalSpeed.y = 0.0f;
	finalSpeed.z = direction.z * SPEED_VALUE * gameObject->rigidbody->Acceleration;

	gameObject->rigidbody->Speed += finalSpeed;

	if (move == true) { rotationDirection = direction; }

	//Rotation
	{
		//float rot = (rotationDirection.y - model->gameObject->transform->Rotation.y);	//‰ñ“]Šp“x‚Ì·•ª
		//if (rot > 90) { rot = rot - 360; }
		//if (rot < -90) { rot = rot + 360; }

		//if (fabs(rot) < D3DXToRadian(2)) { model->gameObject->transform->Rotation.y = rotationDirection.y; }
		//else { model->gameObject->transform->Rotation.y += rot * 0.3f * Time::fixedTimeScale; }



		//if (Input::GetButtonPress(FORWARD_KEYMAP))	{ angle = 0.0f;				cameraF *= 1.0f;	}
		//if (Input::GetButtonPress(BACK_KEYMAP))		{ angle = D3DX_PI;			cameraF *= -1.0f;	}
		//if (Input::GetButtonPress(LEFT_KEYMAP))		{ angle = -D3DX_PI / 2.0f;	cameraR *= -1.0f;	}
		//if (Input::GetButtonPress(RIGHT_KEYMAP))	{ angle = D3DX_PI / 2.0f;	cameraR *= 1.0f;	}
		//
		//if		(Input::GetButtonPress(FORWARD_KEYMAP)	&& Input::GetButtonPress(LEFT_KEYMAP))	{ angle = -D3DX_PI / 4.0f;			}
		//else if (Input::GetButtonPress(FORWARD_KEYMAP)	&& Input::GetButtonPress(RIGHT_KEYMAP)) { angle = D3DX_PI / 4.0f;			}
		//if		(Input::GetButtonPress(BACK_KEYMAP)		&& Input::GetButtonPress(LEFT_KEYMAP))	{ angle = -3.0f * D3DX_PI / 4.0f;	}
		//else if (Input::GetButtonPress(BACK_KEYMAP)		&& Input::GetButtonPress(RIGHT_KEYMAP)) { angle = 3.0f * D3DX_PI / 4.0f;	}


		//angle = atan2f(rotationDirection.x, rotationDirection.z);

		//D3DXQUATERNION quat;
		//D3DXQuaternionRotationAxis(&quat, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), angle);
		//D3DXQuaternionSlerp(&model->gameObject->transform->Quaternion, &model->gameObject->transform->Quaternion, &quat, 0.2f * Time::fixedTimeScale);
	}

	if (move == true) { model->SetAnimationBlend("Run", true); }
	else {
		model->SetAnimationBlend("Idle", true); 
	}

}

void PlayerMovement::AimingMove()
{
	float lerper = Mathf::Lerp(cameraController->GetFollowSpeed().x, 0.08f, 0.1f);
	cameraController->SetFollowSpeed(D3DXVECTOR3(lerper, cameraController->GetFollowSpeed().y, cameraController->GetFollowSpeed().z));

	float thres;
	thres = gun1->GetMaterial()->GetFloat("_Threshold");
	thres = Mathf::Lerp(thres, 2.0f, timerVector["dissolveSpeedAppear"] * Time::fixedTimeScale);
	gun1->GetMaterial()->SetFloat("_Threshold", thres);

	thres = gun2->GetMaterial()->GetFloat("_Threshold");
	thres = Mathf::Lerp(thres, 2.0f, timerVector["dissolveSpeedAppear"] * Time::fixedTimeScale);
	gun2->GetMaterial()->SetFloat("_Threshold", thres);
	

	D3DXVECTOR2 joystick = D3DXVECTOR2(Input::Horizontal(), Input::Vertical());

	if (fabs(joystick.y) > 0.0f || fabs(joystick.x) > 0.0f) { move = true; }
	else { move = false; }

	if(fabs(joystick.y) > 0.0f && fabs(joystick.x) > 0.0f) { diagonal = true; }
	else { diagonal = false; }

	D3DXVECTOR3 finalSpeed;
	finalSpeed.x = direction.x * SPEED_VALUE * gameObject->rigidbody->Acceleration;
	finalSpeed.y = 0.0f;
	finalSpeed.z = direction.z * SPEED_VALUE * gameObject->rigidbody->Acceleration;

	gameObject->rigidbody->Speed += finalSpeed;

	D3DXVECTOR3 face =  *aimPoint - gameObject->transform->Position;
	D3DXVec3Normalize(&face, &face);
	rotationDirection = face;


	if (move == true)
	{
		//if (diagonal == false)
		//{
		//	if (joystick.y > 0.0f) { model->SetAnimationBlend("Forward_Jog", true); }
		//	else if (joystick.y < 0.0f) { model->SetAnimationBlend("Backward_Jog", true); }
		//	if (joystick.x < 0.0f) { model->SetAnimationBlend("Left_Jog", true); }
		//	else if (joystick.x > 0.0f) { model->SetAnimationBlend("Right_Jog", true); }
		//}
		//else
		//{
		//	if (joystick.y > 0.0f)
		//	{
		//		if (joystick.x < 0.0f) { model->SetAnimationBlend("Forward_Left_Jog", true); }
		//		else if (joystick.x > 0.0f) { model->SetAnimationBlend("Forward_Right_Jog", true); }
		//	}
		//	else if (joystick.y < 0.0f)
		//	{
		//		if (joystick.x < 0.0f) { model->SetAnimationBlend("Backward_Left_Jog", true); }
		//		else if (joystick.x > 0.0f) { model->SetAnimationBlend("Backward_Right_Jog", true); }
		//	}
		//}

		D3DXVECTOR3 forward = camera->GetForward();
		D3DXVECTOR3 right = camera->GetRight();
		D3DXVec3Normalize(&face, &face);

		//D3DXVECTOR3 forward = gameObject->transform->GetForwardDirection();
		//D3DXVECTOR3 right = gameObject->transform->GetRightDirection();
		float forwardProduct = D3DXVec3Dot(&direction, &forward);
		float rightProduct = D3DXVec3Dot(&direction, &right);
			
		if (forwardProduct >= 0.92f) { model->SetAnimationBlend("Forward_Jog", true); }
		//else if (forwardProduct < 0.92f && forwardProduct >= 0.38f) { model->SetAnimationBlend("Forward_Right_Jog", true); }
		//else if (forwardProduct < 0.38f && forwardProduct >= -0.38f) { model->SetAnimationBlend("Right_Jog", true); }
		//else if (forwardProduct < -0.38f && forwardProduct >= -0.92f) { model->SetAnimationBlend("Backward_Right_Jog", true); }
		else if (forwardProduct < -0.92f) { model->SetAnimationBlend("Backward_Jog", true); }
		else
		{
		     if (forwardProduct < 0.92f && forwardProduct >= 0.38f) 
			 {
				 if (rightProduct < 0.92f && rightProduct >= 0.38f) { model->SetAnimationBlend("Forward_Right_Jog", true); }
				 else if (rightProduct < -0.38f && rightProduct >= -0.92f) { model->SetAnimationBlend("Forward_Left_Jog", true); }
			 }
			 else if (forwardProduct < 0.38f && forwardProduct >= -0.38f)
			 {
				 if (rightProduct >= 0.92f) { model->SetAnimationBlend("Right_Jog", true); }
				 else if (rightProduct < 0.92f) { model->SetAnimationBlend("Left_Jog", true); }
			 }
			 else if (forwardProduct < -0.38f && forwardProduct >= -0.92f)
			 {
				 if (rightProduct < 0.92f && rightProduct >= 0.38f) { model->SetAnimationBlend("Backward_Right_Jog", true); }
				 else if (rightProduct < -0.38f && rightProduct >= -0.92f) { model->SetAnimationBlend("Backward_Left_Jog", true); }
			 }
		}

	}
	else { model->SetAnimationBlend("Idle", true); }

	timerVector["shootCountdown"] += Time::deltaTime;

	if (/*ImGui::IsKeyPressed((ImGuiKey)655)*/Input::GetButtonPress(SHOOT_KEYMAP) && timerVector["shootCountdown"] >= timerVector["shootCooldown"])
	{
		timerVector["shootCountdown"] = 0.0f;

		GAMEOBJECT* spawner;
		if (gunSelection == true)
		{
			gunSelection = false;
			spawner = gun1->GetChildren()[0]; 
			gun1->transform->Rotation.z = 51.5f;
		}
		else
		{
			gunSelection = true;
			spawner = gun2->GetChildren()[0]; 
			gun2->transform->Rotation.z = 49.5f;
		}

		face = *aimPoint - spawner->transform->GlobalPosition;
		D3DXVec3Normalize(&face, &face);
		rotationDirection = face;

		BULLET* bullet = Manager::GetScene()->AddGameObject<BULLET>("Bullet(Clone)", GAMEOBJECT_LAYER);
		if (bullet != nullptr)
		{
			bullet->transform->Position = spawner->transform->GlobalPosition;
			bullet->rigidbody->Speed = face * bullet->speed;
		}
	}
}

void PlayerMovement::Roll()
{
	timerVector["invincibilityCounter"] += Time::deltaTime;
	if (timerVector["invincibilityCounter"] <= Time::deltaTime * 10.0f)
	{
		invincibility = true;
	}
	else
	{
		invincibility = false;
	}

	float lerper = Mathf::Lerp(cameraController->GetFollowSpeed().x, 0.012f, 0.5f);
	cameraController->SetFollowSpeed(D3DXVECTOR3(lerper, cameraController->GetFollowSpeed().y, cameraController->GetFollowSpeed().z));

	if (model->GetAnimationOver("Roll") == true)
	{
		playerState = NORMAL_MOVE_PS;
	}
}

void PlayerMovement::Death()
{
	model->SetAnimationBlend("Death", false, 0.4f);

	timerVector["deathTimer"] += Time::deltaTime;

	if (timerVector["deathTimer"] >= 3.58f)
	{
		if (model->GetStop() == false)
		{
			model->SetStop(true);
		}
	}
}

void PlayerMovement::Hit()
{
	model->SetAnimationBlend("Hit", false, 0.4f);

	timerVector["hitTimer"] += Time::deltaTime;

	if (timerVector["hitTimer"] >= 0.5f)
	{
		playerState = NORMAL_MOVE_PS;
		timerVector["hitTimer"] = 0.0f;
		model->SetAnimationBlend("Idle", true, 0.25f);
	}
}