#pragma once
#include "Entity.h"
class Tile : public Entity
{
public:
	std::vector<Entity> managedEntities;
};

