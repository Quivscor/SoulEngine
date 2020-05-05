#pragma once
#include "Core.h"
#include "Component.h"
#include "Collider.h"
#include "Transform.h"
#include "Mesh.h"
#include "EntityManager.h"

class ScriptableObject : public Component
{
public:
	ScriptableObject();
	~ScriptableObject();

private:
	virtual void Start() {};
	virtual void Update() {};
	virtual void OnTriggerEnter(std::shared_ptr<Collider> other) {};
	virtual void OnTriggerStay(std::shared_ptr<Collider> other) {};
	virtual void OnTriggerExit(std::shared_ptr<Collider> other) {};

	virtual ComponentType GetComponentType() const override { return ComponentType::ScriptableObjectComponent; }
};