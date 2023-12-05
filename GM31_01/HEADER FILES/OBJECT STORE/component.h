#pragma once
#include <unordered_map>

#include "gameobject.h"
#include "debugManager.h"
#include "modelReader.h"
#include "soundReader.h"
#include "textureReader.h"

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

//Animation Based Classes
class Data
{
public:
	float move;
	float* pointer;

public:

	Data() {}
	Data(float m, float* p) { move = m; pointer = p; }

};
class AnimationData
{
public:
	int frame;
	std::vector<Data> angle;

public:

	AnimationData() {}
	AnimationData(int f, Data a) { frame = f; angle.push_back(a); }

	AnimationData operator=(AnimationData const& obj)
	{
		AnimationData result;
		result.frame = obj.frame;
		result.angle = obj.angle;
		return result;
	}

};
class Animation
{
public:
	enum ANIMATION_STATUS
	{
		STANDBY = 0,
		PLAYBACK,
		END,
		LOOP,

		ANIMATION_STATUS_MAX
	};

public:
	int index;
	int animationSize;
	int keyframes;

	float timer;

	ANIMATION_STATUS status;

	std::string name;

	GAMEOBJECT* gameObject;

	std::vector<std::string> keyName;
	std::vector<AnimationData> data;

public:

	virtual void Start() = 0;

};

//Base Component Virtual Class
class Component
{
protected:
	bool multiple = false;
	bool enabled = true;

public:
	std::string name;

	GAMEOBJECT* gameObject;

public:

	bool GetMultipleSet()		{ return multiple; }
	bool GetEnabled()			{ return enabled; }
	bool* GetEnabledPointer()	{ return &enabled; }

	void SetEnabled(bool value) { enabled = value; }

	Component() { enabled = true; multiple = false; name = ""; }
	virtual ~Component() {}

	virtual void Start() = 0;
	virtual void End() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void EngineDisplay() = 0;

	virtual void ObjectJointer() {}
	virtual void OnTriggerEnter(GAMEOBJECT* obj)	{}
	virtual void OnCollisionEnter(GAMEOBJECT* obj)	{}

	virtual void StrayUpdate() {}


	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(
			CEREAL_NVP(enabled)
		);
	}

};

//Components
class Transform : public Component
{
public:
	bool culling;

	float drawRadius;

	D3DXVECTOR3 Position;
	D3DXVECTOR3 Rotation;
	D3DXVECTOR3 Scale;

	D3DXVECTOR3 GlobalPosition;

	D3DXQUATERNION Quaternion;

	aiMatrix4x4* boneMatrix;

public:

	Transform() { name = "Transform"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	void FaceTowards(GAMEOBJECT* object);
	void FaceTowards(D3DXVECTOR3 value);
	void FaceTowardsXYZ(GAMEOBJECT* object);
	float FaceInDirection();
	D3DXMATRIX FaceInDirectionXYZ();
	D3DXVECTOR3 GetForwardDirection();
	D3DXVECTOR3 GetUpDirection();
	D3DXVECTOR3 GetRightDirection();
	float DistanceFrom(GAMEOBJECT* from);
	float DistanceFromWithY0(GAMEOBJECT* from);

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(cereal::make_nvp("Component",cereal::virtual_base_class<Component>(this)),
			CEREAL_NVP(Position),
			CEREAL_NVP(Rotation),
			CEREAL_NVP(Scale)
		);
	}

	//void serialize(std::ostream& ar) const override 
	//{
	//	cereal::JSONOutputArchive archive(ar);
	//	archive(cereal::virtual_base_class<Component>(this),
	//		CEREAL_NVP(Position),
	//		CEREAL_NVP(Rotation),
	//		CEREAL_NVP(Scale)
	//	);
	//}

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

	Rigidbody() { name = "Rigidbody"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	void SetFollowTarget(GAMEOBJECT* target, float dist) { FollowTarget = target; distance = dist; }
};
class Afterimage : public Component
{
public:
	int imageNum;

public:

	Afterimage() { name = "Afterimage"; }

	void Start() override;
	void End() override {}
	void Update() override;
	void Draw() override {}

	void EngineDisplay() override;

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

	SpriteRenderer() { name = "SpriteRenderer"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

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

	int modelIndex;

	float blendRate;
	float blendSpeed;
	float frame;
	float time;

	std::string animationName;
	std::string animationBlendName;

	AnimationModel* m_Model;
	Model* m_Model_obj;

public:
	bool loop;

public:

	MeshFilter() { name = "MeshFilter"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	const char* GetCurrentAnimation() { if (m_Model != nullptr) { return animationBlendName.c_str(); } return ""; }
	bool GetAnimationOver(const char* name);

	void SetAnimationBlend(const char* name, bool lp = false, float speed = 0.05f);

	void SetFBX(bool value);
	bool GetFBX() { return fbx; }

	void SetModel(ModelReader::READ_MODEL_FBX index);
	void SetModel(ModelReader::READ_MODEL_OBJ index);

	void SetDefaultAnimation(std::string value) { if (m_Model != nullptr) { animationName = value; animationBlendName = value; } }

	AnimationModel* GetModel() { return m_Model; }
	Model* GetModel(int i) { return m_Model_obj; }
};
class SphereCollider : public Component
{
private:
	bool isTrigger;
	bool isKinematic;

	float CollisionSize;

	GAMEOBJECT* colliderObject;

public:
	float scaleOffset;

public:

	SphereCollider() { name = "SphereCollider"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	void SetBone(MeshFilter* model, const char* bone);

	bool GetIsTrigger() { return isTrigger; }
	bool GetIsKinematic() { return isKinematic; }
	float GetCollisionSize() { return CollisionSize; }
	GAMEOBJECT* GetColliderObject() { return colliderObject; }

	void SetIsTrigger(bool value) { isTrigger = value; }
	void SetIsKinematic(bool value) { isKinematic = value; }
	void SetCollisionSize(float s) { CollisionSize = s; }

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(cereal::virtual_base_class<Component>(this),
			CEREAL_NVP(isTrigger),
			CEREAL_NVP(isKinematic),
			CEREAL_NVP(CollisionSize),
			CEREAL_NVP(scaleOffset)
		);
	}
};
class Camera : public Component
{
protected:
	bool shake;

	int power;
	int targetIndex;

	float fov;
	float len;
	float rad;

	//float limit;
	//float shakeCounter;
	//float shakeValue;
	//float time;

	D3DXVECTOR3 shakeOffset;
	float shakeTime;
	float frequency;
	D3DXVECTOR3 shakeAmplitude;

	D3DXVECTOR3 Up;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 at;

	D3DXMATRIX  mtxView;
	D3DXMATRIX  mtxProjection;

	GAMEOBJECT* Target;

public:

	Camera() { name = "Camera"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	float GetLen() { return len; }
	D3DXVECTOR3 GetAt() { return at; }
	D3DXVECTOR3 GetRot() { return rot; }
	D3DXMATRIX GetViewMatrix() { return mtxView; }
	D3DXMATRIX GetProjectionMatrix() { return mtxProjection; }
	GAMEOBJECT* GetTarget() { return Target; }

	void SetAt(D3DXVECTOR3 value) { at = value; }
	void SetViewMatrix(D3DXMATRIX value) { mtxView = value; }
	void SetProjectionMatrix(D3DXMATRIX value) { mtxProjection = value; }
	void SetTarget(GAMEOBJECT* value);


	D3DXVECTOR3 GetForward();
	D3DXVECTOR3 GetUp();
	D3DXVECTOR3 GetRight();
	void CameraShake(D3DXVECTOR3 value, float f = 1.5f);
	bool CheckView(Transform* target);

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(cereal::virtual_base_class<Component>(this),
			CEREAL_NVP(fov)
		);
	}
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

	Plane() { name = "Plane"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

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

	Billboard() { name = "Billboard"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

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

	void EngineDisplay() override {}

	void StrayUpdate() override {}
};
class ParticleSystem : public Component
{
private:
	int numberOfObjects;
	int numberOfObjectsToAdd;
	int texture;

	D3DXVECTOR3 direction;

	std::vector<PARTICLE*> objects;

	SCENE* scene;

public:
	bool burst;
	bool loop;
	bool rotateRandom;

	float rotationSpeed;

public:

	ParticleSystem() { name = "ParticleSystem"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	int GetNumberOfObjects() { return numberOfObjects; }

	void SetTexture(TextureReader::READ_TEXTURE text);
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

	Number() { name = "Number"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	void SetDigits(int num);
	void SetNumber(int num) { number = num; }
	void SetNumberColor(D3DXCOLOR value);

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(cereal::virtual_base_class<Component>(this),
			CEREAL_NVP(number),
			CEREAL_NVP(digits)
		);
	}
};
class AudioListener : public Component
{
private:
	float startArea;
	float endArea;

	float scaleOffset;

	GAMEOBJECT* area1;
	GAMEOBJECT* area2;

public:

	AudioListener() { name = "AudioListener"; }

	void Start();
	void End();
	void Update();
	void Draw();

	void EngineDisplay() override;

	float GetStartArea() { return startArea; }
	float GetEndArea() { return endArea; }

	void SetArea(float start = -1.0f, float end = -1.0f) 
	{
		if (start >= 0.0f) { startArea = start; }
		if (end >= 0.0f) { endArea = end; }
	}
};
class AudioSource : public Component
{
private:
	bool loop;
	bool parentActive;
	bool playOnAwake;
	bool threeDimension;

	int soundIndex;

	float volume;
	float volumePercentage;

	Audio* clip;
	std::vector<AudioListener*> listeners;

public:

	AudioSource() { name = "AudioSource"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	bool GetLoop() { return loop; }
	bool GetPlayOnAwake() { return playOnAwake; }
	bool GetThreeDimension() { return threeDimension; }
	float GetVolume() { return volume; }
	Audio* GetClip() { return clip; }

	void SetLoop(bool value) { loop = value; }
	void SetPlayOnAwake(bool value) { playOnAwake = value; }
	void SetThreeDimension(bool value) { threeDimension = value; }
	void SetVolume(float value) { volume = value; }
	void SetClip(SoundReader::READ_SOUND index);

	void Play(bool l = false, float v = 1.0f);
	void PlayOneShot(Audio* audio, float v = 1.0f);

	static SOUND* PlayClipAtPoint(Audio* clip, D3DXVECTOR3 position, float volume);

};
class Animator : public Component
{
private:
	class AnimatorPoint
	{
	public:
		bool clicked;
		bool hovered;
		bool held;

		D3DXVECTOR2 minusDirection;
		D3DXVECTOR2 plusDirection;

		ImPlotPoint point;

	public:

		AnimatorPoint()
		{
			clicked = false;
			hovered = false;
			held = false;

			minusDirection = D3DXVECTOR2(0.0f, 0.0f);
			plusDirection = D3DXVECTOR2(0.0f, 0.0f);
		}

		AnimatorPoint(float x, float y)
		{
			clicked = false;
			hovered = false;
			held = false;

			minusDirection = D3DXVECTOR2(0.0f, 0.0f);
			plusDirection = D3DXVECTOR2(0.0f, 0.0f);

			point.x = x;
			point.y = y;
		}

	};

private:
	int animIndex;

	std::vector<Animation*> animation;

public:

	Animator() { name = "Animator"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

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

		animation[animIndex]->animationSize = animation[animIndex]->data.back().frame;
		animation[animIndex]->keyframes = (int)animation[animIndex]->data[0].angle.size();
	}
	//void AddAnimation(Animation* anim, int index, Animation::ANIMATION_STATUS stat = Animation::STANDBY)
	//{
	//	animation.push_back(anim);
	//	animIndex = index;
	//	InitAnimation(stat);
	//}
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

	template<class T>
	T* AddAnimation(Animation::ANIMATION_STATUS stat = Animation::STANDBY)
	{
		T* buff = new T();
		buff->gameObject = gameObject;
		buff->Start();

		animation.push_back(buff);
		animIndex = (int)animation.size() - 1;
		InitAnimation(stat);

		return buff;
	}
};
class Shadow : public Component
{
public:
	float shadowHeight;

	SHADOW* shadow;

public:

	Shadow() { name = "Shadow"; }

	void Start() override;
	void End() override {}
	void Update() override;
	void Draw() override {}

	void EngineDisplay() override;
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

	MeshField() { name = "MeshField"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	D3DXVECTOR3 GetNormal() { return Normal; }
	float GetHeight(D3DXVECTOR3 position);
	ID3D11Buffer* GetVertexBuffer() { return VertexBuffer; }

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

	FollowObject() { name = "FollowObject"; }

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

	void EngineDisplay() override {}

	bool GetFollowBool() { return follow; }
	GAMEOBJECT* GetTarget() { return target; }

	void SetFollowBool(bool value) { follow = value; }
	void SetTarget(GAMEOBJECT* value) { target = value; }
};
class BoxCollider : public Component
{
private:
	bool isTrigger;
	bool isKinematic;

	D3DXVECTOR3 CollisionSize;

public:
	float scaleOffset;

	GAMEOBJECT* collider = nullptr;

public:

	BoxCollider() { name = "BoxCollider"; }

	void Start() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void EngineDisplay() override;

	void SetBone(MeshFilter* model, const char* bone);

	bool GetIsTrigger() { return isTrigger; }
	bool GetIsKinematic() { return isKinematic; }
	D3DXVECTOR3 GetCollisionSize() { return CollisionSize; }
	GAMEOBJECT* GetColliderObject() { return collider; }

	void SetIsTrigger(bool value) { isTrigger = value; }
	void SetIsKinematic(bool value) { isKinematic = value; }
	void SetCollisionSize(D3DXVECTOR3 s) { CollisionSize = s; }

};