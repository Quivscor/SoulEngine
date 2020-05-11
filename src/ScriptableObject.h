#pragma once
#include "Core.h"
#include "Component.h"
#include "Collider.h"
#include "Transform.h"
#include "Mesh.h"
//#include "Model.h"
#include "EntityManager.h"
#include "GameLogic.h"

//class Model;

class GameLogic;
#define thisEntity EntityManager::GetInstance()->GetEntity(GetOwnerID())

class ScriptableObject : public Component
{
public:
	friend class Collider;
	friend class GameLogic;

	ScriptableObject();
	~ScriptableObject();

	virtual ComponentType GetComponentType() const override { return ComponentType::ScriptableObjectComponent; }

protected:
	virtual void Start() {};
	virtual void Update() {};
	virtual void OnTriggerEnter(std::shared_ptr<Collider> other) {};
	virtual void OnTriggerStay(std::shared_ptr<Collider> other) {};
	virtual void OnTriggerExit(std::shared_ptr<Collider> other) {};
};