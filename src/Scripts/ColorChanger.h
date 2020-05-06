#pragma once
#include "ScriptableObject.h"
class ColorChanger : public ScriptableObject
{
public:
	ColorChanger();
	~ColorChanger();

protected:
	virtual void OnTriggerEnter(std::shared_ptr<Collider> other) override;
	virtual void OnTriggerStay(std::shared_ptr<Collider> other) override;
	virtual void OnTriggerExit(std::shared_ptr<Collider> other) override;
};