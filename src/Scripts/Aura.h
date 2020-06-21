#ifndef Aura_HEADER
#define Aura_HEADER
//#include "ScriptableObject.h"
#include "Enemy.h"
#include "Character.h"

class Aura// : public ScriptableObject
{
public:
	Aura() {};
	~Aura() {};

	virtual void LoadAura(std::shared_ptr<Enemy> enemyScr, std::shared_ptr<Character> enemyCharacter) {};
	virtual std::string ToString() { return ""; };

private:

};
#endif