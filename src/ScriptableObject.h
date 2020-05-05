#pragma once
#include "Core.h"
#include "Component.h"
#include "Collider.h"

class ScriptableObject : public Component
{
public:
	ScriptableObject();
	~ScriptableObject();

private:
	virtual void OnTriggerEnter(std::shared_ptr<Collider> other);
	virtual void OnTriggerStay(std::shared_ptr<Collider> other);
	virtual void OnTriggerExit(std::shared_ptr<Collider> other);
};