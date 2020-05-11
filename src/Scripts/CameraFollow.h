#pragma once
#include "ScriptableObject.h"

class CameraFollow : public ScriptableObject
{
public:
	CameraFollow();
	~CameraFollow();

	std::shared_ptr<Transform> objectToFollow;

protected:
	virtual void Start() override;
	virtual void Update() override;
	//virtual void OnTriggerEnter(std::shared_ptr<Collider> other) override;
	//virtual void OnTriggerStay(std::shared_ptr<Collider> other) override;
	//virtual void OnTriggerExit(std::shared_ptr<Collider> other) override;
};