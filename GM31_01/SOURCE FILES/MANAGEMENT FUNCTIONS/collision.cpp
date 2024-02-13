#include "manager.h"

#define BOUND_CONST 1.01f

bool ColOBBs(GAMEOBJECT* obb1, GAMEOBJECT* obb2, D3DXVECTOR3 size1, D3DXVECTOR3 size2);
float LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 = 0);

//void COLLISION::Update()
//{
//	SCENE* game = Manager::GetScene();
//	for (auto object : game->GetGameObjectList(GAMEOBJECT_LAYER))
//	{
//		if (object->GetComponent<SphereCollider>() != nullptr)
//		{
//			SphereCollider* collider = object->GetComponent<SphereCollider>();
//			for (auto anotherObject : game->GetGameObjectList(GAMEOBJECT_LAYER))
//			{
//				if (anotherObject != object)
//				{
//					if (anotherObject->GetComponent<SphereCollider>() != nullptr)
//					{
//						SphereCollider* anotherCollider = anotherObject->GetComponent<SphereCollider>();
//
//						D3DXVECTOR3 distance;
//						distance = object->transform->GlobalPosition - anotherObject->transform->GlobalPosition;
//						float length = D3DXVec3Length(&distance);
//
//						if ((length < (collider->GetCollisionSize() + anotherCollider->GetCollisionSize())))
//						{
//							collider->GetColliderObject()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
//
//							if (collider->GetIsTrigger() == true || anotherCollider->GetIsTrigger() == true)
//							{
//								for (auto com : object->GetComponents<Script>())
//								{
//									if (com != nullptr)
//									{
//										com->OnTriggerEnter(anotherObject);
//									}
//								}
//							}
//							else
//							{
//								float overlap = (collider->GetCollisionSize() + anotherCollider->GetCollisionSize()) - length;
//								D3DXVECTOR3 dir = collider->gameObject->transform->Position - anotherCollider->gameObject->transform->Position;
//								D3DXVec3Normalize(&dir, &dir);
//								//object->transform->Position += dir * overlap;
//
//								//Dynamic Collision
//								GAMEOBJECT* temp = object;
//								if (object->Parent != nullptr) { temp = object->Parent; }
//
//								if (collider->GetIsKinematic() == false)
//								{
//									temp->transform->Position.x += dir.x * overlap;
//									temp->transform->Position.z += dir.z * overlap;
//
//									if (temp->GetFreezeY() == false)
//									{
//										if ((temp->transform->Position.y + dir.y * overlap) >= temp->GetDefaultY()) { temp->transform->Position.y += dir.y * overlap; }
//										else { temp->transform->Position.y = temp->GetDefaultY(); }
//									}
//								}
//
//								for (auto com : object->GetComponents<Script>())
//								{
//									if (com != nullptr)
//									{
//										com->OnCollisionEnter(anotherObject);
//									}
//								}
//
//								//Physics Collision
//								////Rigidbody*
//								//D3DXVECTOR3 v1 = object->rigidbody->Speed;
//								//D3DXVECTOR3 v2 = anotherObject->rigidbody->Speed;
//								//float m1 = object->rigidbody->mass;
//								//float m2 = anotherObject->rigidbody->mass;
//
//								//v1.y = 0.0f;
//								//v2.y = 0.0f;
//
//								//object->rigidbody->Speed.x = ((m1 - BOUND_CONST * m2) * v1.x) + (((1 + BOUND_CONST) * m2 * v2.x) / (m1 + m2));
//								//object->rigidbody->Speed.z = ((m1 - BOUND_CONST * m2) * v1.z) + (((1 + BOUND_CONST) * m2 * v2.z) / (m1 + m2));
//								//anotherObject->rigidbody->Speed.x = ((m2 - BOUND_CONST * m1) * v2.x) + (((1 + BOUND_CONST) * m1 * v1.x) / (m1 + m2));
//								//anotherObject->rigidbody->Speed.z = ((m2 - BOUND_CONST * m1) * v2.z) + (((1 + BOUND_CONST) * m1 * v1.z) / (m1 + m2));
//								
//							}
//						}
//						else
//						{
//							collider->GetColliderObject()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//						}
//					}
//				}
//			}
//		}
//	}
//}

void COLLISION::Update()
{
	SCENE* game = Manager::GetScene();
	for (auto object : game->GetGameObjectList(COLLIDER_LAYER))
	{
		//Sphere Collider
		{
			SphereCollider* collider = object->Parent->GetComponent<SphereCollider>();
			if (collider != nullptr)
			{
				for (auto anotherObject : game->GetGameObjectList(COLLIDER_LAYER))
				{
					if (anotherObject != object)
					{
						SphereCollider* anotherCollider = anotherObject->Parent->GetComponent<SphereCollider>();

						if (collider == nullptr || anotherCollider == nullptr) { continue; }

						bool sameLayer = false;
						for (COLLISION_LAYER layer : collider->GetCollisionLayer())
						{
							if (layer == anotherCollider->GetSelfLayer())
							{
								sameLayer = true;
								break;
							}
						}

						if (sameLayer == false) { continue; }

						if (anotherCollider != nullptr && anotherCollider->gameObject != collider->gameObject)
						{
							D3DXVECTOR3 distance;
							distance = object->transform->GlobalPosition - anotherObject->transform->GlobalPosition;
							float length = D3DXVec3Length(&distance);

							if ((length < (collider->GetCollisionSize() + anotherCollider->GetCollisionSize())))
							{
								collider->GetColliderObject()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

								if (collider->GetIsTrigger() == true || anotherCollider->GetIsTrigger() == true)
								{
									for (auto com : object->Parent->GetComponents<Script>())
									{
										if (com != nullptr)
										{
											if (collider->GetCollision() == false) { com->OnTriggerEnter(anotherObject);  }
											else { com->OnTriggerStay(anotherObject); }
										}
									}
									collider->SetCollision(true);
								}
								else
								{
									float overlap = (collider->GetCollisionSize() + anotherCollider->GetCollisionSize()) - length;
									D3DXVECTOR3 dir = object->transform->GlobalPosition - anotherObject->transform->GlobalPosition;
									D3DXVec3Normalize(&dir, &dir);
									//object->transform->Position += dir * overlap;

									D3DXVECTOR3 mover = dir * overlap * Time::deltaTime * 3.0f;

									//Dynamic Collision
									GAMEOBJECT* temp = collider->gameObject;
									while (temp->Parent != nullptr) { temp = temp->Parent; }

									if (collider->GetIsKinematic() == false)
									{
										temp->transform->Position.x += mover.x;
										temp->transform->Position.z += mover.z;

										if (temp->GetFreezeY() == false)
										{
											if ((temp->transform->Position.y + mover.y) >= temp->GetDefaultY()) { temp->transform->Position.y += mover.y; }
											else { temp->transform->Position.y = temp->GetDefaultY(); }
										}
									}

									for (auto com : object->Parent->GetComponents<Script>())
									{
										if (com != nullptr)
										{
											if (collider->GetCollision() == false) { com->OnCollisionEnter(anotherObject); }
											else { com->OnCollisionStay(anotherObject); }
										}
									}
									collider->SetCollision(true);

									//Physics Collision
									/*
									////Rigidbody*
									//D3DXVECTOR3 v1 = object->rigidbody->Speed;
									//D3DXVECTOR3 v2 = anotherObject->rigidbody->Speed;
									//float m1 = object->rigidbody->mass;
									//float m2 = anotherObject->rigidbody->mass;

									//v1.y = 0.0f;
									//v2.y = 0.0f;

									//object->rigidbody->Speed.x = ((m1 - BOUND_CONST * m2) * v1.x) + (((1 + BOUND_CONST) * m2 * v2.x) / (m1 + m2));
									//object->rigidbody->Speed.z = ((m1 - BOUND_CONST * m2) * v1.z) + (((1 + BOUND_CONST) * m2 * v2.z) / (m1 + m2));
									//anotherObject->rigidbody->Speed.x = ((m2 - BOUND_CONST * m1) * v2.x) + (((1 + BOUND_CONST) * m1 * v1.x) / (m1 + m2));
									//anotherObject->rigidbody->Speed.z = ((m2 - BOUND_CONST * m1) * v2.z) + (((1 + BOUND_CONST) * m1 * v1.z) / (m1 + m2));
									*/

								}
							}
							else
							{
								collider->GetColliderObject()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

								if (collider->GetCollision() == true)
								{
									for (auto com : object->Parent->GetComponents<Script>())
									{
										if (com != nullptr)
										{
											if (collider->GetIsTrigger() == true || anotherCollider->GetIsTrigger() == true)
											{
												com->OnTriggerExit(anotherObject);
											}
											else
											{
												com->OnCollisionExit(anotherObject);
											}
										}
									}
								}

								collider->SetCollision(false);
							}

						}
					}
				}
			}
		}

		//Box Collider
		//if(game->GetGameObjectList(COLLIDER_LAYER).size() >= 2)
		{
			BoxCollider* collider = object->Parent->GetComponent<BoxCollider>();
			if (collider != nullptr)
			{
				for (auto anotherObject : game->GetGameObjectList(COLLIDER_LAYER))
				{
					if (anotherObject != object)
					{
						BoxCollider* anotherCollider = anotherObject->Parent->GetComponent<BoxCollider>();

						if (anotherCollider != nullptr)
						{
							//Collision
							bool hit = ColOBBs(object, anotherObject, collider->GetCollisionSize(), anotherCollider->GetCollisionSize());
							{
								//Normal Box Collision
								{
									/*
									D3DXVECTOR3 box1 = object->transform->GlobalPosition;
									D3DXVECTOR3 size1 = collider->GetCollisionSize() * 0.5f;

									D3DXVECTOR3 box2 = anotherObject->transform->GlobalPosition;
									D3DXVECTOR3 size2 = anotherCollider->GetCollisionSize() * 0.5f;

									if ((box1.x - size1.x) <= (box2.x + size2.x) && (box1.x + size1.x) > (box2.x - size2.x))
									{
										if ((box1.z - size1.z) <= (box2.z + size2.z) && (box1.z + size1.z) > (box2.z - size2.z))
										{
											if ((box1.y - size1.y) <= (box2.y + size2.y) && (box1.y + size1.y) > (box2.y - size2.y))
											{
												hit = true;
											}
											else
											{
												hit = false;
											}
										}
									}
									*/
								}

								//Oriented Bounding Box Collision
								{
									//D3DXVECTOR3 box1 = object->transform->GlobalPosition;
									//D3DXVECTOR3 size1 = collider->GetCollisionSize();

									//D3DXVECTOR3 box2 = anotherObject->transform->GlobalPosition;
									//D3DXVECTOR3 size2 = anotherCollider->GetCollisionSize();

									//D3DXVECTOR3 up1 = object->transform->GetUpDirection();
									//D3DXVECTOR3 right1 = object->transform->GetRightDirection();
									//D3DXVECTOR3 forward1 = object->transform->GetForwardDirection();

									//D3DXVECTOR3 up2 = anotherObject->transform->GetUpDirection();
									//D3DXVECTOR3 right2 = anotherObject->transform->GetRightDirection();
									//D3DXVECTOR3 forward2 = anotherObject->transform->GetForwardDirection();
									//
									//D3DXVECTOR3 distance = box1 - box2;

									//float x;
									//float y;
									//float z;

									//bool hit1 = false;
									//bool hit2 = false;
									//
									////box 1
									//{
									//	x = D3DXVec3Dot(&distance, &right2);
									//	y = D3DXVec3Dot(&distance, &up2);
									//	z = D3DXVec3Dot(&distance, &forward2);

									//	//add the radius of the sphere to the size to get a collision with an obb and a sphere
									//	if (fabs(x) < size2.x)
									//	{
									//		if (fabs(y) < size2.y)
									//		{
									//			if (fabs(z) < size2.z)
									//			{
									//				hit1 = true;
									//			}
									//		}
									//	}
									//}

									////box 2
									//{
									//	x = D3DXVec3Dot(&distance, &right1);
									//	y = D3DXVec3Dot(&distance, &up1);
									//	z = D3DXVec3Dot(&distance, &forward1);

									//	if (fabs(x) < size1.x)
									//	{
									//		if (fabs(y) < size1.y)
									//		{
									//			if (fabs(z) < size1.z)
									//			{
									//				hit2 = true;
									//			}
									//		}
									//	}
									//}

									//if (hit1 == true && hit2 == true)
									//{
									//	hit = true;
									//}

								}

								//Divided Axis Oriented Bounding Box Collision
								{
									//D3DXVECTOR3 NAe1 = object->transform->GetRightDirection();
									//D3DXVECTOR3 A_e1 = NAe1 * size1.x;
									//float rA = D3DXVec3Length(&A_e1);
									//float rB = LenSegOnSeparateAxis(&NAe1, &right2, &up2, &forward2);
									//float L = fabs(D3DXVec3Dot(&distance, &NAe1));
									//if (L < rA + rB) { hit2 = true; } // 衝突していない
								}
							}

							if (hit == true)
							{
								collider->GetColliderObject()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

								if (collider->GetIsTrigger() == true || anotherCollider->GetIsTrigger() == true)
								{
									for (auto com : object->Parent->GetComponents<Script>())
									{
										if (com != nullptr)
										{
											com->OnTriggerEnter(anotherObject);
										}
									}
								}
								else
								{
									for (auto com : object->Parent->GetComponents<Script>())
									{
										if (com != nullptr)
										{
											com->OnCollisionEnter(anotherObject);
										}
									}

								}
							}
							else
							{
								collider->GetColliderObject()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
							}

						}
					}
				}
			}
		}
	}
}

float LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは標準化されていること
	float r1 = fabs(D3DXVec3Dot(Sep, e1));
	float r2 = fabs(D3DXVec3Dot(Sep, e2));
	float r3 = e3 ? (fabs(D3DXVec3Dot(Sep, e3))) : 0;
	return r1 + r2 + r3;
}

// OBB v.s. OBB
bool ColOBBs(GAMEOBJECT* obb1, GAMEOBJECT* obb2, D3DXVECTOR3 size1, D3DXVECTOR3 size2)
{
	// 各方向ベクトルの確保
	// （N***:標準化方向ベクトル）
	D3DXVECTOR3 NAe1 = obb1->transform->GetRightDirection(),	Ae1 = NAe1 * size1.x;
	D3DXVECTOR3 NAe2 = obb1->transform->GetUpDirection(),		Ae2 = NAe2 * size1.y;
	D3DXVECTOR3 NAe3 = obb1->transform->GetForwardDirection(),	Ae3 = NAe3 * size1.z;
	D3DXVECTOR3 NBe1 = obb2->transform->GetRightDirection(),	Be1 = NBe1 * size2.x;
	D3DXVECTOR3 NBe2 = obb2->transform->GetUpDirection(),		Be2 = NBe2 * size2.y;
	D3DXVECTOR3 NBe3 = obb2->transform->GetForwardDirection(),	Be3 = NBe3 * size2.z;
	D3DXVECTOR3 Interval = obb1->transform->GlobalPosition - obb2->transform->GlobalPosition;

	// 分離軸 : Ae1
	FLOAT rA = D3DXVec3Length(&Ae1);
	FLOAT rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	FLOAT L = fabs(D3DXVec3Dot(&Interval, &NAe1));
	if (L > rA + rB)
		return false; // 衝突していない

	 // 分離軸 : Ae2
	rA = D3DXVec3Length(&Ae2);
	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &NAe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Ae3
	rA = D3DXVec3Length(&Ae3);
	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &NAe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be1);
	L = fabs(D3DXVec3Dot(&Interval, &NBe1));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be2);
	L = fabs(D3DXVec3Dot(&Interval, &NBe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be3);
	L = fabs(D3DXVec3Dot(&Interval, &NBe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : C11
	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &NAe1, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C12
	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C13
	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C21
	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C22
	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C23
	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C31
	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C32
	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C33
	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離平面が存在しないので「衝突している」
	return true;
}