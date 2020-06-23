#pragma once

#include "Core.h"
#include "Renderer.h"
#include "AssetManager.h"
#include "Physics.h"
#include "Enemy.h"
#include "Character.h"
#include "Entity.h"
#include "EntityManager.h"
#include <time.h>
#include "Model.h"
#include "Village.h"
#include "Scripts/EnemyWeapon.h"
#include "Scripts/WeaponOnTheGround.h"
#include "Scripts/AuraBonusDamage.h"
#include "Scripts/AuraBonusHealth.h"
#include "Scripts/AuraRunningSpeed.h"

class MapGenerator
{
public:
	MapGenerator(Renderer * renderer, AssetManager * assetManager, Physics * physics, Shader * animShader, Shader * grassShader, GameLogic * gameLogic, std::shared_ptr<EntityManager> m_EntityManager, std::shared_ptr<Entity> player);
	~MapGenerator();
	void Generate();
	void Clear();
	std::shared_ptr <Entity> player;
	int mapSizeX;
	int mapSizeY;
private:
	Renderer* renderer;
	AssetManager * assetManager;
	Physics * physics;
	Shader * animShader;
	Shader * grassShader;
	GameLogic* gameLogic;
	std::shared_ptr<EntityManager> m_EntityManager;
	
	
	bool **mapShape = false;
	int tilesNumber = 0;
	int startVillagesCount = 5;
	int villagesCount = startVillagesCount;
	std::string mapTiles[4] = { "GrassLand","Forest1","Forest2","Mountains" };
	std::string **generatedMap;
	Model* tileModels[14];
	std::vector<glm::vec2> treeCollider;
	std::vector<glm::vec2> waterCollider;
	std::vector<glm::vec2> rockCollider;
	std::vector<glm::vec2> characterCollider;
	std::vector<glm::vec2> house1Collider;
	std::vector<glm::vec2> house2Collider;
	std::vector<glm::vec2> enemyCollider;
	
	// functions
	Model * FindModelByName(Model * array[], std::string name);
	void LoadMapModels();
	void PrepareColliders();
	int GenerateRandomNumber(int min, int max);
	void ShowMapShape();
	void CreateMapShape();
	void CountTiles();
	void CreateVillages();
	void ShowVillagesMap();
	void GenerateRandomTiles();
	void ShowMapTiles();
};
