#include "../manager.h"
#include "../MANAGEMENT FUNCTIONS/scene.h"

#define BOUND_CONST 1.01f

void COLLISION::Init()
{

}

void COLLISION::Update()
{
	SCENE* game = Manager::GetScene();
	for (auto object : game->GetGameObjectList(GAMEOBJECT_LAYER))
	{
		if (object->GetComponent<SphereCollider>() != nullptr)
		{
			SphereCollider* collider = object->GetComponent<SphereCollider>();
			for (auto anotherObject : game->GetGameObjectList(GAMEOBJECT_LAYER))
			{
				if (anotherObject != object)
				{
					if (anotherObject->GetComponent<SphereCollider>() != nullptr)
					{
						SphereCollider* anotherCollider = anotherObject->GetComponent<SphereCollider>();

						D3DXVECTOR3 distance;
						distance = object->transform->GlobalPosition - anotherObject->transform->GlobalPosition;
						float length = D3DXVec3Length(&distance);

						if ((length < (collider->GetCollisionSize() + anotherCollider->GetCollisionSize())))
						{
							collider->GetColliderObject()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

							if (collider->GetIsTrigger() == true || anotherCollider->GetIsTrigger() == true)
							{
								for (auto com : object->GetComponents<Script>())
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
								GAMEOBJECT* temp = object;
								if (object->Parent != nullptr) { temp = object->Parent; }

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

								for (auto com : object->GetComponents<Script>())
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
}

//void COLLISION::Update()
//{
//	SCENE* game = Manager::GetScene();
//	for (auto object : game->GetGameObjectList(COLLIDER_LAYER))
//	{
//		
//			SphereCollider* collider = object->Parent->GetComponent<SphereCollider>();
//			for (auto anotherObject : game->GetGameObjectList(COLLIDER_LAYER))
//			{
//				if (anotherObject != object)
//				{
//					
//						SphereCollider* anotherCollider = anotherObject->Parent->GetComponent<SphereCollider>();
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
//								GAMEOBJECT* temp = collider->gameObject;
//								while (temp->Parent != nullptr) { temp = temp->Parent; }
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
//					
//				}
//			}
//		
//	}
//}
