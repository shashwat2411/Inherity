#pragma once
#include <unordered_map>

#include "gameobject.h"

#include <string>

//前方宣言
class PARTICLE;
class DIGIT;
class SHADOW;
class SCENE;
class Material;
class AnimationModel;
class Model;
class Audio;
class SOUND;

#define COLLIDERS

#define VIEW_NEAR_Z 0.5f
#define VIEW_FAR_Z 1000.0f

#define SPEED_VALUE 0.01f
#define ACCELERATION_VALUE 1.1f

#define GRAVITY_CONSTANT 1.0f
#define GRAVITY_ACCELERATION 1.1f

#define TILES 20

//Data Class
class Data
{
public:
	Data() {}
	Data(float m, float* p) { move = m; pointer = p; }
	float move;
	float* pointer;
};
class AnimationData
{
public:
	AnimationData() {}
	AnimationData(int f, Data a) { frame = f; angle.push_back(a); }
	int frame;
	std::vector<Data> angle;

	AnimationData operator=(AnimationData const& obj)
	{
		AnimationData result;
		result.frame = obj.frame;
		result.angle = obj.angle;
		return result;
	}
};

//Base Component Virtual Class
class Component
{
protected:
	bool multiple = false;
	bool ignorePause = false;
	bool enabled = true;

public:
	GAMEOBJECT* gameObject;

	bool GetMultipleSet() { return multiple; }
	bool GetIgnorePause() { return ignorePause; }
	bool GetEnabled() { return enabled; }

	void SetEnabled(bool value) { enabled = value; }

	virtual ~Component() {}
	virtual void Start() = 0;
	virtual void End() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void OnTriggerEnter(GAMEOBJECT* obj) {}
	virtual void OnCollisionEnter(GAMEOBJECT* obj) {}

	virtual void StrayUpdate() {}

};

//Components
class Transform : public Component
{
public:

public:
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Rotation;
	D3DXVECTOR3 Scale;

	D3DXQUATERNION Quaternion;

	D3DXVECTOR3 GlobalPosition;

	float drawRadius;
	bool culling;

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void FaceTowards(GAMEOBJECT* object);
	void FaceTowards(D3DXVECTOR3 value);
	void FaceTowardsXYZ(GAMEOBJECT* object);
	float FaceInDirection();
	D3DXMATRIX FaceInDirectionXYZ();
	D3DXVECTOR3 GetForwardDirection();
	D3DXVECTOR3 GetRightDirection();
	float DistanceFrom(GAMEOBJECT* from);
	float DistanceFromWithY0(GAMEOBJECT* from);
};
class Rigidbody : public Component
{
public:
	bool useGravity;

	float Acceleration;
	float mass;
	float distance;
	float groundLevel;

	D3DXVECTOR3 Speed;

	GAMEOBJECT* FollowTarget;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void SetFollowTarget(GAMEOBJECT* target, float dist) { FollowTarget = target; distance = dist; }
};
class Afterimage : public Component
{
public:
	int imageNum;

public:

	void Start() override;
	void End() override {}
	void Update() override;
	void Draw() override {}

	void SetAfterimageNumber(int value) { imageNum = value; }
};
class SpriteRenderer : public Component
{
private:
	D3DXVECTOR2 Size;
	D3DXVECTOR2 TexCoord;
	D3DXCOLOR Color;

	ID3D11Buffer* VertexBuffer = nullptr;

public:
	bool animate;

	int count;
	int elementsX;
	int elementsY;

	float barOffsetRight;
	float barOffsetLeft;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	D3DXVECTOR2 GetSize() { return Size; }
	D3DXVECTOR2 GetTexCoord() { return TexCoord; }
	D3DXCOLOR GetColor() { return Color; }

	void SetSize(D3DXVECTOR2 value) { Size = value; }
	void SetTexCoord(D3DXVECTOR2 value) { TexCoord = value; }
	void SetColor(D3DXCOLOR value) { Color = value; }

};
class MeshFilter :public Component
{
private:
	bool fbx;

	float blendRate;
	float blendSpeed;
	float time;

	std::string animationName;
	std::string animationBlendName;

	AnimationModel* m_Model;
	Model* m_Model_obj;

public:
	bool loop;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	const char* GetCurrentAnimation() { return animationBlendName.c_str(); }
	bool GetAnimationOver(const char* name);

	void SetAnimationBlend(const char* name, bool lp = false, float speed = 0.05f);

	void SetFBX(bool value);
	bool GetFBX() { return fbx; }

	void SetModel(AnimationModel* value) { SetFBX(true); m_Model = value; }
	void SetModel(Model* value) { SetFBX(false); m_Model_obj = value; }

	void SetDefaultAnimation(std::string value) { animationName = value; animationBlendName = value; }

	AnimationModel* GetModel() { return m_Model; }
	Model* GetModel(int i) { return m_Model_obj; }
};
class SphereCollider : public Component
{
private:
	bool isTrigger;
	bool isKinematic;

	float CollisionSize;

public:
	float scaleOffset;

	GAMEOBJECT* collider = nullptr;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	bool GetIsTrigger() { return isTrigger; }
	bool GetIsKinematic() { return isKinematic; }
	float GetCollisionSize() { return CollisionSize; }
	GAMEOBJECT* GetColliderObject() { return collider; }

	void SetIsTrigger(bool value) { isTrigger = value; }
	void SetIsKinematic(bool value) { isKinematic = value; }
	void SetCollisionSize(float s) { CollisionSize = s; }

};
class Camera : public Component
{
protected:
	bool shake;

	float fov;
	float len;
	float rad;

	float limit;
	float shakeCounter;
	float shakeValue;
	float time;

	D3DXVECTOR3 Up;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 at;

	D3DXMATRIX  mtxView;
	D3DXMATRIX  mtxProjection;

public:
	GAMEOBJECT* Target;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	D3DXMATRIX GetViewMatrix() { return mtxView; }
	D3DXMATRIX GetProjectionMatrix() { return mtxProjection; }

	void SetViewMatrix(D3DXMATRIX value) { mtxView = value; }
	void SetProjectionMatrix(D3DXMATRIX value) { mtxProjection = value; }


	D3DXVECTOR3 GetForward();
	D3DXVECTOR3 GetRight();
	void CameraShake(float value, float t = 15.0f / FRAME_RATE);
	bool CheckView(Transform* target);
};
class Plane : public Component
{
protected:
	D3DXVECTOR3 Normal;

	ID3D11Buffer* VertexBuffer = nullptr;

public:
	D3DXVECTOR2 Size;
	D3DXVECTOR2 TexCoord;

	GAMEOBJECT* collisionObject;

public:
	~Plane() {}

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	D3DXVECTOR3 GetNormal() { return Normal; }

	void SetNormal(D3DXVECTOR3 value) { Normal = value; }
};
class Billboard : public Component
{
private:
	bool animate;
	bool atc;

	int Count;
	int elementsX;
	int elementsY;

	D3DXVECTOR2 Size;
	D3DXVECTOR2 TexCoord;
	D3DXVECTOR2 Offset;

	D3DXVECTOR3 Normal;

	ID3D11Buffer* VertexBuffer = nullptr;

public:
	bool flip;

	GAMEOBJECT* collisionObject;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	enum COOD { X = 0, Y };

	bool GetAnimate() { return animate; }
	bool GetATC() { return atc; }
	int GetElements(COOD value) { if (value == X) { return elementsX; } else { return elementsY; } }
	D3DXVECTOR2 GetSize() { return Size; }
	D3DXVECTOR3 GetNormal() { return Normal; }

	void SetAnimate(bool value) { animate = value; }
	void SetATC(bool value) { atc = value; }
	void SetElements(int x, int y) { elementsX = x; elementsY = y; }
	void SetSize(D3DXVECTOR2 offset) { Size = offset; }
	void SetOffset(D3DXVECTOR2 offset) { Offset = offset; }
	void SetOffset(COOD value) { if (value == X) { Offset.x = Size.x; } else { Offset.y = Size.x; } }
	void SetNormal(D3DXVECTOR3 value) { Normal = value; }

};
class Script :public Component
{
protected:
	std::unordered_map<std::string, float>timerVector;

public:

	void Start() override {}
	void End() override {}
	void Update() override {}
	void Draw() override {}

	void StrayUpdate() override {}
};
class ParticleSystem : public Component
{
private:
	int numberOfObjects;
	int numberOfObjectsToAdd;

	D3DXVECTOR3 direction;

	ID3D11ShaderResourceView* texture;

	std::vector<PARTICLE*> objects;

	SCENE* scene;

public:
	bool burst;
	bool loop;
	bool rotateRandom;

	float rotationSpeed;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	int GetNumberOfObjects() { return numberOfObjects; }

	void SetTexture(ID3D11ShaderResourceView* text);
	void SetLife(const float value);
	void SetSpeed(const float value);
	void SetParticleCount(int value);


	void Burst();
};
class Number : public Component
{
private:
	int digits;
	int number;

	std::vector<DIGIT*> numbers;
public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void SetDigits(int num);
	void SetNumber(int num) { number = num; }
	void SetNumberColor(D3DXCOLOR value);
};
class AudioSource : public Component
{
private:
	bool loop;
	bool parentActive;
	bool playOnAwake;

	float volume;

public:
	Audio* clip;

public:

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	bool GetLoop() { return loop; }
	bool GetPlayOnAwake() { return playOnAwake; }
	float GetVolume() { return volume; }

	void SetLoop(bool value) { loop = value; }
	void SetPlayOnAwake(bool value) { playOnAwake = value; }
	void SetVolume(float value) { volume = value; }

	void Play(bool l = false, float v = 1.0f);
	void PlayOneShot(Audio* audio, float v = 1.0f);

	static SOUND* PlayClipAtPoint(Audio* clip, D3DXVECTOR3 position, float volume);

};
class Animation : public Component
{
public:
	enum ANIMATION_STATUS
	{
		STANDBY = 0,
		PLAYBACK,
		END,
		LOOP
	};

public:
	int index;
	int animationSize;
	int keyframes;

	float timer;

	ANIMATION_STATUS status;

	std::vector<AnimationData> data;

	void Start() override
	{
		data.push_back(AnimationData(0, Data(0.0f, &gameObject->transform->Rotation.x)));
		data.push_back(AnimationData(20, Data(50.0f, &gameObject->transform->Rotation.x)));
		data.push_back(AnimationData(40, Data(-120.0f, &gameObject->transform->Rotation.x)));
		data.push_back(AnimationData(60, Data(30.0f, &gameObject->transform->Rotation.x)));
		data.push_back(AnimationData(80, Data(-70.0f, &gameObject->transform->Rotation.x)));

		data[0].angle.push_back(Data(0.0f, &gameObject->transform->Rotation.y));
		data[1].angle.push_back(Data(50.0f, &gameObject->transform->Rotation.y));
		data[2].angle.push_back(Data(-120.0f, &gameObject->transform->Rotation.y));
		data[3].angle.push_back(Data(60.0f, &gameObject->transform->Rotation.y));
		data[4].angle.push_back(Data(-20.0f, &gameObject->transform->Rotation.y));

		index = 0;
		timer = 0.0f;
		status = Animation::STANDBY;
	}


	void End() override {}
	void Update() override {}
	void Draw() override {}
};
class Animator : public Component
{
private:
	int animIndex;

	std::vector<Animation*> animation;

public:
	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	int GetCurrentIndex() { return animIndex; }
	Animation::ANIMATION_STATUS GetAnimationState(int index)
	{
		Animation::ANIMATION_STATUS stat = animation[index]->status;
		if (animation[index]->status == Animation::PLAYBACK || animation[index]->status == Animation::LOOP)
		{
			stat = Animation::PLAYBACK;
		}
		return stat;
	}

	void InitAnimation(Animation::ANIMATION_STATUS stat = Animation::STANDBY)
	{
		animation[animIndex]->index = 0;
		animation[animIndex]->timer = 0;
		animation[animIndex]->status = stat;
	}
	void AddAnimation(Animation* anim, int index, Animation::ANIMATION_STATUS stat = Animation::STANDBY)
	{
		animation.push_back(anim);
		animIndex = index;
		InitAnimation(stat);
	}
	void PlayAnimation(int index, Animation::ANIMATION_STATUS stat = Animation::PLAYBACK)
	{
		if (animation[animIndex]->status != Animation::LOOP)
		{
			animIndex = index;
			animation[animIndex]->status = stat;
		}
	}
	void PauseAnimation(int index)
	{
		animation[index]->status = Animation::STANDBY;
	}
	void StopAnimation(int index)
	{
		animation[index]->index = 0;
		animation[index]->timer = 0;
		PauseAnimation(index);
	}
	void AnimationTransition(int index, int anotherIndex)
	{
		PauseAnimation(anotherIndex);
		animation[index]->timer = animation[anotherIndex]->animationSize - animation[anotherIndex]->timer;
		animation[index]->index = animation[anotherIndex]->keyframes - 2 - animation[anotherIndex]->index;
		StopAnimation(anotherIndex);

		PlayAnimation(index);
	}//(次のアニメーション, 止めるアニメーション)

};
class Shadow : public Component
{
public:
	float shadowHeight;

	SHADOW* shadow;

public:
	~Shadow() {}

	void Start() override;
	void End() override {}
	void Update() override;
	void Draw() override {}
};
class MeshField : public Component
{
protected:
	D3DXVECTOR3 Normal;

	ID3D11Buffer* VertexBuffer{};
	ID3D11Buffer* IndexBuffer{};

	VERTEX_3D m_Vertex[TILES + 1][TILES + 1]{};

public:
	D3DXVECTOR2 Size;
	D3DXVECTOR2 TexCoord;

	GAMEOBJECT* collisionObject;

public:
	~MeshField() {}

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	D3DXVECTOR3 GetNormal() { return Normal; }
	float GetHeight(D3DXVECTOR3 position);

	void SetNormal(D3DXVECTOR3 value) { Normal = value; }

	void RecreateField();
};
class FollowObject : public Component
{
public:
	bool follow;
	
	float followSpeed;

	GAMEOBJECT* target;

public:

	void Start() override
	{
		follow = true;
		
		followSpeed = 0.04f;

		target = nullptr;
	}
	void End() override
	{
		target = nullptr;
		delete target;
	}
	void Update() override
	{
		if (follow == true)
		{
			if (target == nullptr) { return; }

			gameObject->transform->FaceTowards(target);

			D3DXVECTOR3 direction = target->transform->Position - gameObject->transform->Position;
			D3DXVec3Normalize(&direction, &direction);
			direction *= followSpeed;

			gameObject->transform->Position.x += direction.x;
			gameObject->transform->Position.y += direction.y;
			gameObject->transform->Position.z += direction.z;

			//if ((gameObject->transform->Position.y + direction.y) > gameObject->GetDefaultY()) { gameObject->transform->Position.y += direction.y; }
			//else { gameObject->transform->Position.y = gameObject->GetDefaultY(); }
		}
	}
	void Draw() override {}

	bool GetFollowBool() { return follow; }
	GAMEOBJECT* GetTarget() { return target; }

	void SetFollowBool(bool value) { follow = value; }
	void SetTarget(GAMEOBJECT* value) { target = value; }
};

//Camera Types
class TopDownCamera : public Camera
{
private:

public:

	void Update() override;

};
class ThirdPersonCamera : public Camera
{
private:

public:
	
	void Update() override;

};
class FirstPersonCamera : public Camera
{
private:

public:

	void Update() override;

};
class OpenWorldCamera : public Camera
{
private:

public:

	void Update() override;

};
class RevolutionCamera : public Camera
{
private:

public:

	void Update() override;

};