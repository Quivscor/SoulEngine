#ifndef Village_HEADER
#define Village_HEADER
#include "ScriptableObject.h"
#include "Enemy.h"
#include "Aura.h"

class Village : public ScriptableObject
{
public:
	Village();
	~Village();

	std::vector<std::shared_ptr<Enemy>> enemiesInVillage;
	std::vector<Aura*> auras;

private:
	virtual void Start() override;
	virtual void Update() override;
	virtual void OnTriggerEnter(std::shared_ptr<Collider> other) override;
	virtual void OnTriggerExit(std::shared_ptr<Collider> other) override;

	bool aurasLoaded = false;
};
#endif