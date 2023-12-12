#pragma once
//#include <list>
//#include <vector>
#include "saveFunctions.h"

#define MAX_AFTERIMAGES 1

class Component;
class Transform;
class Rigidbody;
class Shadow;
class Material;


class GAMEOBJECT
{
protected:
	bool active;
	bool billboard;
	bool collide;

	bool depth;
	bool depthShadow;
	bool reflection;

	bool destroy;

	bool faceInDirection;
	bool quaternion;

	bool freezeX;
	bool freezeY;
	bool freezeZ;

	int id;
	int parentIndex;
	int RingCounter;

	std::string tag;

	float defaultY;

	D3DXCOLOR Color;

	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11InputLayout* VertexLayout;

	std::list<Component*> components;
	Material* material;

	D3DXMATRIX WorldMatrix[MAX_AFTERIMAGES];
	D3DXMATRIX ScaleMatrix;
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX TransformMatrix;

public:
	float cameraDistance;

	Transform*	transform;
	Rigidbody*	rigidbody;
	Shadow*		shadow;
	GAMEOBJECT* Parent;
	std::vector<GAMEOBJECT*>Children;

	//bool operator<(const GAMEOBJECT* other) const{
	//	// Compare points based on their Z-coordinates in descending order
	//	return this->cameraDistance > other->cameraDistance;
	//}

public:

	//Functions
	void Initialize();
	void UnInitialize();
	void Draw();
	bool Remove();
	void Destroy(bool value = true);
	void EngineDisplay();

	//Pure Virtual Functions
	virtual void Init()		{ Initialize(); }
	virtual void Start()	{ Init(); }
	virtual void Update() = 0;

	//Getter Functions
	ID3D11VertexShader*	GetVertexShader()	{ return VertexShader; }
	ID3D11PixelShader*	GetPixelShader()	{ return PixelShader; }
	ID3D11InputLayout*	GetVertexLayout()	{ return VertexLayout; }

	ID3D11VertexShader**	GetVertexShaderPointer()	{ return &VertexShader; }
	ID3D11PixelShader**		GetPixelShaderPointer()		{ return &PixelShader; }
	ID3D11InputLayout**		GetVertexLayoutPointer()	{ return &VertexLayout; }

	D3DXMATRIX GetScaleMatrix()					{ return ScaleMatrix; }
	D3DXMATRIX GetRotationMatrix()				{ return RotationMatrix; }
	D3DXMATRIX GetTransformMatrix()				{ return TransformMatrix; }
	D3DXMATRIX GetWorldMatrix(int index = 0)	{ return WorldMatrix[index]; }

	bool		GetActive()			{ return active; }
	bool		GetBillboard()		{ return billboard; }
	bool		GetDepthShadow()	{ return depthShadow; }
	bool		GetReflection()		{ return reflection; }

	bool		GetFaceInDirection() { return faceInDirection; }

	bool		GetFreezeX() { return freezeX; }
	bool		GetFreezeY() { return freezeY; }
	bool		GetFreezeZ() { return freezeZ; }

	int						GetID()				{ return id; }
	int						GetRingCounter()	{ return RingCounter; }
	float					GetDefaultY()		{ return defaultY; }
	D3DXCOLOR				GetColor()			{ return Color; }
	Material*				GetMaterial()		{ return material; }
	std::string				GetTag()			{ return tag; }
	std::list<Component*>	GetComponentList()	{ return components; }


	//Setter Functions
	void SetWorldMatrix(D3DXMATRIX value, int i = 0)	{ WorldMatrix[i] = value; }
	void SetScaleMatrix(D3DXMATRIX value)				{ ScaleMatrix = value; }
	void SetRotationMatrix(D3DXMATRIX value)			{ RotationMatrix = value; }
	void SetTransformMatrix(D3DXMATRIX value)			{ TransformMatrix = value; }
	GAMEOBJECT* SetParent(GAMEOBJECT* parent);

	void SetActive(bool value)			{ active = value; }
	void SetBillboard(bool value)		{ billboard = value; }
	void SetDepth(bool value)			{ depth = value; }
	void SetDepthShadow(bool value)		{ depthShadow = value; }
	void SetFaceInDirection(bool value)	{ faceInDirection = value; }
	void SetReflection(bool value)		{ reflection = value; }

	void SetID(int value)			{ id = value; }
	void SetRingCounter(int value)	{ RingCounter = value; }
	void SetTag(const char* value)	{ tag = value; }
	void SetTag(std::string value)	{ tag = value; }
	void SetColor(D3DXCOLOR color)	{ Color = color; }



	//Material Function
	template<class T>
	T* AddMaterial()
	{
		T* buff = new T();
		buff->gameObject = this;
		buff->Start();


		if (material == nullptr)
		{
			material = buff;
		}
		else
		{
			delete material;
			material = buff;
		}
		return buff;
	}

	//Component Functions
	template<class T>
	T* GetComponent()
	{
		for (auto com : components)
		{
			T* buff = dynamic_cast<T*>(com);
			if (buff != nullptr)
			{
				return buff;
			}
		}
		return nullptr;
	}

	template<class T>
	T* AddComponent()
	{
		T* buff = new T();
		buff->gameObject = this;
		T* temp = nullptr;
		temp = GetComponent<T>();
		buff->Start();
		if (temp == nullptr)
		{
			components.push_back(buff);
		}
		else
		{
			if (temp->GetMultipleSet() == true)
			{
				components.push_back(buff);
			}
		}
		return buff;
	}

	template<class T>
	void RemoveComponent()
	{
		T* buff = GetComponent<T>();
		if (buff != nullptr)
		{
			buff->End();
			components.remove(buff);
			delete buff;
		}
	}

	void RemoveComponent(Component* com);

	template<class T>
	T* GetComponentInChildren()
	{
		for (auto child : Children)
		{
			for (auto com : child->components)
			{
				T* buff = dynamic_cast<T*>(com);
				if (buff != nullptr)
				{
					return buff;
				}
			}
		}
	}

	template<typename T>
	std::vector<T*> GetComponents()
	{
		std::vector<T*> objects;
		for (auto obj : components)
		{
			T* buff = dynamic_cast<T*>(obj);
			if (buff != nullptr)
			{
				objects.push_back((T*)buff);
			}
		}
		return objects;
	}


	//Serialization
	template<class Archive>
	void serialize(Archive & archive);
};

