#ifndef Village_HEADER
#define Village_HEADER
#include "ScriptableObject.h"
#include "Enemy.h"

class Village : public ScriptableObject
{
public:
	Village();
	~Village();

	std::vector<std::shared_ptr<Enemy>> enemiesInVillage;

private:
	virtual void Start() override;
	virtual void OnTriggerEnter(std::shared_ptr<Collider> other) override;
	virtual void OnTriggerExit(std::shared_ptr<Collider> other) override;
};
#endif