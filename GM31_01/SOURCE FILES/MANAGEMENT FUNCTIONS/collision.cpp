#include "manager.h"

#define BOUND_CONST 1.01f

void COLLISION::Init()
{

}

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

						if (anotherCollider != nullptr)
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
											com->OnTriggerEnter(anotherObject);
										}
									}
								}
								else
								{
									float overlap = (collider->GetCollisionSize() + anotherCollider->GetCollisionSize()) - length;
									D3DXVECTOR3 dir = collider->gameObject->transform->Position - anotherCollider->gameObject->transform->Position;
									D3DXVec3Normalize(&dir, &dir);
									//object->transform->Position += dir * overlap;

									//Dynamic Collision
									GAMEOBJECT* temp = collider->gameObject;
									while (temp->Parent != nullptr) { temp = temp->Parent; }

									if (collider->GetIsKinematic() == false)
									{
										temp->transform->Position.x += dir.x * overlap;
										temp->transform->Position.z += dir.z * overlap;

										if (temp->GetFreezeY() == false)
										{
											if ((temp->transform->Position.y + dir.y * overlap) >= temp->GetDefaultY()) { temp->transform->Position.y += dir.y * overlap; }
											else { temp->transform->Position.y = temp->GetDefaultY(); }
										}
									}

									for (auto com : object->Parent->GetComponents<Script>())
									{
										if (com != nullptr)
										{
											com->OnCollisionEnter(anotherObject);
										}
									}

									//Physics Collision
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

		//Box Collider
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
							bool hit = false;

							//Collision
							{
								//Normal Box Collision
								{
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
								}

								//Vertex Box Collision
								{
									/*D3DXVECTOR3 box2 = anotherObject->transform->GlobalPosition;
									D3DXVECTOR3 size2 = anotherCollider->GetCollisionSize() * 0.5f;

									for (int i = 0; i < 8; i++)
									{
										D3DXVECTOR3 box1 = collider->GetVertex()[i];

										if (box1.x <= (box2.x + size2.x) && box1.x > (box2.x - size2.x))
										{
											if (box1.z <= (box2.z + size2.z) && box1.z > (box2.z - size2.z))
											{
												if (box1.y <= (box2.y + size2.y) && box1.y > (box2.y - size2.y))
												{
													hit = true;
												}
												else
												{
													hit = false;
												}
											}
										}
									}*/
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