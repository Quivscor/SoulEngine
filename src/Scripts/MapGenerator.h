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
class MapGenerator
{
public:
	MapGenerator(Renderer * renderer, AssetManager * assetManager, Physics * physics, Shader * animShader, Shader * grassShader, GameLogic * gameLogic, std::shared_ptr<EntityManager> m_EntityManager, std::shared_ptr<Entity> player);
	~MapGenerator();
	void Generate();
	void Clear();
	std::shared_ptr <Entity> player;
private:
	Renderer* renderer;
	AssetManager * assetManager;
	Physics * physics;
	Shader * animShader;
	Shader * grassShader;
	GameLogic* gameLogic;
	std::shared_ptr<EntityManager> m_EntityManager;
	
	int mapSizeX;
	int mapSizeY;
	bool **mapShape;
	int startVillagesCount = 6;
	int villagesCount = startVillagesCount;
	std::string mapTiles[4] = { "GrassLand","Forest1","Forest2","Mountains" };
	std::string **generatedMap;
	Model* tileModels[13];
	std::vector<glm::vec2> colliderShape;
	std::vector<glm::vec2> characterCollider;
	
	// functions
	Model * FindModelByName(Model * array[], std::string name);
	void LoadMapModels();
	void PrepareColliders();
	int GenerateRandomNumber(int min, int max);
	void ShowMapShape();
	void CreateMapShape();
	void CreateVillages();
	void ShowVillagesMap();
	void GenerateRandomTiles();
	void ShowMapTiles();
};
