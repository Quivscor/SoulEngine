#include "MapGenerator.h"

MapGenerator::MapGenerator(Renderer* renderer, AssetManager* assetManager, Physics* physics, Shader* animShader, Shader* grassShader, GameLogic* gameLogic, std::shared_ptr<EntityManager> m_EntityManager, std::shared_ptr<Entity> player) 
{
	this->renderer = renderer;
	this->assetManager = assetManager;
	this->physics = physics;
	this->animShader = animShader;
	this->grassShader = grassShader;
	this->gameLogic = gameLogic;
	this->m_EntityManager = m_EntityManager;
	this->player = player;
}

void MapGenerator::Generate()
{
	std::vector<std::shared_ptr<Entity>> map;
	float pos[3];
	float scale[3];
	std::string name;
	Shader* shaderForEnemy = new Shader("./res/shaders/anim.vert", "./res/shaders/anim.frag");

	PrepareColliders();
	LoadMapModels();

	Model* grassLeaf = assetManager->LoadModel("./res/models/tiles/Grass/GrassFromGalapagos.obj");
	InstanceManager* grassM = new InstanceManager(grassLeaf);
	std::shared_ptr <InstanceManager> grassManager(grassM);
	name = "GrassLeaf";
	int numberOfGrass = grassManager->amount;
	srand(time(NULL));
	for (int i = 0; i < numberOfGrass; i++)
	{
		float randomX = rand() % 40 + (rand() % 100) / 100.f;
		if (rand() % 2 == 0)
			randomX *= -1;
		float randomY = rand() % 40 + (rand() % 100) / 100.f;
		if (rand() % 2 == 0)
			randomY *= -1;
		glm::vec3 pos(randomX, 0, randomY);
		glm::vec3 scale(0.05, 0.3, 0.05);
		glm::vec3 rot(0, 0, 0);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(pos));
		model = glm::scale(model, glm::vec3(scale));
		std::shared_ptr<Entity> object = m_EntityManager->CreateEntity<Entity>();
		grassManager->instanceModels[i] = model;

		object->AddComponent<Mesh>();
		object->GetComponent<Mesh>()->indices = (grassLeaf)->GetMeshes()[0].indices;
		object->GetComponent<Mesh>()->vertices = (grassLeaf)->GetMeshes()[0].vertices;
		object->GetComponent<Mesh>()->material = (grassLeaf)->GetMeshes()[0].material;
		object->GetComponent<Mesh>()->setupMesh();
		grassManager->m_mesh = object->GetComponent<Mesh>().get();

	}
	std::cout << sizeof(grassManager->instanceModels);
	grassManager->m_shader = grassShader;
	glGenBuffers(1, &grassManager->buffer);
	glBindBuffer(GL_ARRAY_BUFFER, grassManager->buffer);
	glBufferData(GL_ARRAY_BUFFER, numberOfGrass * sizeof(glm::mat4), &grassManager->instanceModels[0], GL_STATIC_DRAW);
	unsigned int VAO = grassLeaf->meshes[0].GetVAO();
	glBindVertexArray(VAO);
	// set attribute pointers for matrix (4 times vec4)
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
	renderer->RegisterManager(grassManager);

	mapSizeX = GenerateRandomNumber(7, 12);
	mapSizeY = GenerateRandomNumber(7, 12);

	mapSizeX = 5;
	mapSizeY = 5;
	std::cout << "\nX: " << mapSizeX << " Y: " << mapSizeY << std::endl;

	generatedMap = new std::string*[mapSizeX];
	for (int i = 0; i < mapSizeX; ++i) {
		generatedMap[i] = new std::string[mapSizeY];
	}
	mapShape = new bool*[mapSizeX];
	for (int i = 0; i < mapSizeX; ++i) {
		mapShape[i] = new bool[mapSizeY];
	}
	//srand(time(NULL));
	CreateMapShape();
	ShowMapShape();
	CreateVillages();
	ShowVillagesMap();
	GenerateRandomTiles();
	ShowMapTiles();

	for (int i = 0; i < mapSizeX; i++)
	{
		for (int j = 0; j < mapSizeY; j++)
		{

			std::ifstream file;
			if (generatedMap[i][j] == "Forest1")
			{
				//std::cout << "\nForest 1 loading \n";
				file.open("./res/maps/TileForest.txt");
				if (!file)
				{
					std::cout << "Unable to open TileForest.txt";
				}
			}
			if (generatedMap[i][j] == "Forest2")
			{
				//std::cout << "Forest 2 loading \n";
				file.open("./res/maps/TileForest2.txt");
				if (!file)
				{
					std::cout << "Unable to open file Forest2.txt";
				}
			}
			if (generatedMap[i][j] == "Village")
			{
				//std::cout << "Village loading \n";
				file.open("./res/maps/TileVillage.txt");
				if (!file)
				{
					std::cout << "Unable to open file TileVillage.txt";
				}
			}
			if (generatedMap[i][j] == "Mountain")
			{
				//std::cout << "Mountain loading \n";
				file.open("./res/maps/TileMountain.txt");
				if (!file)
				{
					std::cout << "Unable to open file TileMountain.txt";
				}
			}
			if (generatedMap[i][j] == "GrassLand")
			{
				//std::cout << "Grassland loading \n";
				file.open("./res/maps/TileGrassLand.txt");
				if (!file)
				{
					std::cout << "Unable to open file TileGrassLand.txt";
				}
			}
			std::shared_ptr<Entity> tile = m_EntityManager->CreateEntity<Entity>();
			tile->AddComponent<Transform>();

			map.push_back(tile);
			renderer->RegisterEntity(tile);
			physics->RegisterEntity(tile);

			if (generatedMap[i][j] == "Village")
			{
				tile->AddComponent<Village>();
				gameLogic->RegisterEntity(tile);
			}

			while (file >> name >> pos[0] >> pos[1] >> pos[2] >> scale[0] >> scale[1] >> scale[2])
			{
				std::shared_ptr<Entity> object = m_EntityManager->CreateEntity<Entity>();

				object->AddComponent<Transform>();
				object->GetComponent<Transform>()->SetParent(tile->GetComponent<Transform>());
				object->GetComponent<Transform>()->SetLocalPosition(glm::vec3(pos[0], pos[1], pos[2]));
				object->GetComponent<Transform>()->SetLocalScale(glm::vec3(scale[0], scale[1], scale[2]));


				object->AddComponent<Mesh>();
				object->GetComponent<Mesh>()->indices = (FindModelByName(tileModels, name))->GetMeshes()[0].indices;
				object->GetComponent<Mesh>()->vertices = (FindModelByName(tileModels, name))->GetMeshes()[0].vertices;
				object->GetComponent<Mesh>()->material = (FindModelByName(tileModels, name))->GetMeshes()[0].material;
				object->GetComponent<Mesh>()->setupMesh();

				if (name == "Grass")
				{
					object->layer = Layer::GroundLayer;
				}
				if (name == "Tree" || name == "Birch")
				{
					object->AddComponent<Collider>();
					object->GetComponent<Collider>()->SetShape(colliderShape);
					object->GetComponent<Collider>()->isStatic = true;
				}
				if (name == "Enemy")
				{
					object->AddComponent<Model>();

					object->GetComponent<Model>()->UseModel((FindModelByName(tileModels, name)));
					object->GetComponent<Mesh>()->SetAll((FindModelByName(tileModels, name))->GetMeshes()[0]);
					object->GetComponent<Mesh>()->material->SetShader(animShader);
					object->GetComponent<Mesh>()->setupMeshfBones();

					object->AddComponent<Collider>();
					object->GetComponent<Collider>()->SetShape(characterCollider);
					object->AddComponent<Character>();
					object->GetComponent<Character>()->playerReference = player;

					player->GetComponent<PlayerEnemyCommunicator>()->EnemySpawned();

					object->layer = EnemyLayer;

					//container for Enemy script
					std::shared_ptr<Entity> enemy = m_EntityManager->CreateEntity<Entity>();
					enemy->AddComponent<Transform>();
					enemy->GetComponent<Transform>()->SetParent(object->GetComponent<Transform>());
					enemy->AddComponent<Enemy>();
					enemy->GetComponent<Enemy>()->animationIdle = FindModelByName(tileModels, "EnemyIdle");
					enemy->GetComponent<Enemy>()->animationRun = FindModelByName(tileModels, "EnemyRun");
					enemy->GetComponent<Enemy>()->animationAttack = FindModelByName(tileModels, "EnemyAttack");
					enemy->GetComponent<Enemy>()->animationDeath = FindModelByName(tileModels, "EnemyDeath");
					enemy->GetComponent<Enemy>()->shader = shaderForEnemy;

					gameLogic->RegisterEntity(enemy);
					physics->RegisterEntity(enemy);

					if (generatedMap[i][j] == "Village")
					{
						tile->GetComponent<Village>()->enemiesInVillage.push_back(enemy->GetComponent<Enemy>());
					}
				}

				map.push_back(object);
				physics->RegisterEntity(object);
				renderer->RegisterEntity(object);

			}

			file.close();

			tile->GetComponent<Transform>()->SetLocalPosition(glm::vec3(i * 16, 0, j * 16));

		}
	}
}
Model* MapGenerator::FindModelByName(Model* array[], std::string name) 
{
	if (name == "Grass")
		return array[0];
	else if (name == "Tree")
		return array[1];
	else if (name == "Birch")
		return array[2];
	else if (name == "Rock1")
		return array[3];
	else if (name == "Rock2")
		return array[4];
	else if (name == "Rock3")
		return array[5];
	else if (name == "House1")
		return array[6];
	else if (name == "House2")
		return array[7];
	else if (name == "Enemy")
		return array[8];
	else if (name == "EnemyIdle")
		return array[9];
	else if (name == "EnemyRun")
		return array[10];
	else if (name == "EnemyAttack")
		return array[11];
	else if (name == "EnemyDeath")
		return array[12];
	else if (name == "GrassLeaf")
		return array[13];
}
void MapGenerator::LoadMapModels()
{
	tileModels[0] = assetManager->LoadModel("./res/models/tiles/Grass/Grass.obj");
	tileModels[1] = assetManager->LoadModel("./res/models/tiles/Tree/Tree.obj");
	tileModels[2] = assetManager->LoadModel("./res/models/tiles/Tree/Birch.obj");
	tileModels[3] = assetManager->LoadModel("./res/models/tiles/Rocks/Rock1.obj");
	tileModels[4] = assetManager->LoadModel("./res/models/tiles/Rocks/Rock2.obj");
	tileModels[5] = assetManager->LoadModel("./res/models/tiles/Rocks/Rock3.obj");
	tileModels[6] = assetManager->LoadModel("./res/models/tiles/Houses/house_1.obj");
	tileModels[7] = assetManager->LoadModel("./res/models/tiles/Houses/house_2.obj");
	tileModels[8] = assetManager->LoadModel("./res/models/player/player_idle.dae");
	tileModels[9] = assetManager->LoadModel("./res/models/player/player_idle.dae");
	tileModels[10] = assetManager->LoadModel("./res/models/player/player_run.dae");
	tileModels[11] = assetManager->LoadModel("./res/models/player/player_attack.dae");
	tileModels[12] = assetManager->LoadModel("./res/models/player/player_death.dae");
}
void MapGenerator::PrepareColliders()
{
	colliderShape.push_back({ -2.0f, -1.25f });
	colliderShape.push_back({ -1.25f, -2.0f });
	colliderShape.push_back({ 1.25f, -2.0f });
	colliderShape.push_back({ 2.0f, -1.25f });
	colliderShape.push_back({ 2.0f, 1.25f });
	colliderShape.push_back({ 1.25f,  2.0f });
	colliderShape.push_back({ -1.25f, 2.0f });
	colliderShape.push_back({ -2.0f, 1.25f });

	characterCollider.push_back({ -8.0f, -5.0f });
	characterCollider.push_back({ -5.0f, -8.0f });
	characterCollider.push_back({ 5.0f, -8.0f });
	characterCollider.push_back({ 8.0f, -5.0f });
	characterCollider.push_back({ 8.0f, 5.0f });
	characterCollider.push_back({ 5.0f,  8.0f });
	characterCollider.push_back({ -5.0f, 8.0f });
	characterCollider.push_back({ -8.0f, 5.0f });
}

int MapGenerator::GenerateRandomNumber(int min, int max)
{
	return  min + (rand() % static_cast<int>(max-1 - min + 1));
}
void MapGenerator::ShowMapShape()
{
	std::cout << "\n Map shape\n";
	for (int i = 0; i < mapSizeX; i++)
	{
		for (int j = 0; j < mapSizeY; j++)
		{
			if (mapShape[i][j] == true)
			{
				std::cout << " O ";
				generatedMap[i][j] = "Land";
			}
			if (mapShape[i][j] == false)
			{
				std::cout << " . ";
				generatedMap[i][j] = "Water";
			}

		}
		std::cout << "\n";
	}
}
void MapGenerator::CreateMapShape()
{
	// making shape of the map - random edge generating
	for (int i = 0; i < mapSizeX; i++)
	{
		for (int j = 0; j < mapSizeY; j++)
		{
			mapShape[i][j] = false;
			if ((i == 0) || (i == (mapSizeX - 1)) || (j == 0) || (j == (mapSizeY - 1)))
			{
				if (rand() % 2 == 1)
				{
					mapShape[i][j] = true;
				}
			}
			else
			{
				mapShape[i][j] = true;
			}

		}
	}
	// deleting corners of map if there is no adiacent tiles
	if (!mapShape[0][1] && !mapShape[1][0]) mapShape[0][0] = false; // left top corner
	if (!mapShape[mapSizeX - 2][0] && !mapShape[mapSizeX - 1][1]) mapShape[mapSizeX - 1][0] = false; // right top corner
	if (!mapShape[0][mapSizeY - 2] && !mapShape[1][mapSizeY - 1]) mapShape[0][mapSizeY - 1] = false; // left bottom corner
	if (!mapShape[mapSizeX - 1][mapSizeY - 2] && !mapShape[mapSizeX - 2][mapSizeY - 1]) mapShape[mapSizeX - 1][mapSizeY - 1] = false; // right bottom corner
}

void MapGenerator::CreateVillages()
{
	// starting tile is a grassland
	generatedMap[mapSizeX / 2][mapSizeY / 2] = "GrassLand";
	// one village is 2 tiles to north from player's spawn
	generatedMap[mapSizeX / 2][mapSizeY / 2 + 2] = "Village";

	while (villagesCount > 0)
	{
		for (int i = 0; i < mapSizeX; i++)
		{
			for (int j = 0; j < mapSizeY; j++)
			{
				if (i == mapSizeX / 2 && j == mapSizeY / 2)
					continue;
				if (villagesCount == 0)
					break;
				if (rand() % 100 <= 5 && mapShape[i][j] && generatedMap[i][j] != "Village")
				{
					generatedMap[i][j] = "Village";
					villagesCount--;
				}
			}

		}
	}
}
void MapGenerator::ShowVillagesMap()
{
	std::cout << "\n Map of villages preset\n";
	for (int i = 0; i < mapSizeX; i++)
	{
		for (int j = 0; j < mapSizeY; j++)
		{
			if (generatedMap[i][j] == "Water") std::cout << " . ";
			if (generatedMap[i][j] == "Land") std::cout << " O ";
			if (generatedMap[i][j] == "Village") std::cout << " V ";
			if (generatedMap[i][j] == "GrassLand") std::cout << " g ";

		}
		std::cout << "\n";
	}
}

void MapGenerator::GenerateRandomTiles()
{
	for (int i = 0; i < mapSizeX; i++)
	{
		for (int j = 0; j < mapSizeY; j++)
		{
			if (mapShape[i][j])
			{
				if (i == mapSizeX / 2 && j == mapSizeY / 2)
					continue;
				if (generatedMap[i][j] != "Village")
				{
					int randomTile = rand() % 4;
					generatedMap[i][j] = mapTiles[randomTile];
				}
			}
			else
			{
				generatedMap[i][j] = "Water";
			}
		}
	}
}

void MapGenerator::ShowMapTiles()
{
	std::cout << "\n Map tiles\n";
	for (int i = 0; i < mapSizeX; i++)
	{
		for (int j = 0; j < mapSizeY; j++)
		{
			if (generatedMap[i][j] == "Village") std::cout << " V ";
			if (generatedMap[i][j] == "Forest1") std::cout << " F ";
			if (generatedMap[i][j] == "Forest2") std::cout << " f ";
			if (generatedMap[i][j] == "GrassLand") std::cout << " g ";
			if (generatedMap[i][j] == "Mountains") std::cout << " m ";
			if (generatedMap[i][j] == "Water") std::cout << " . ";
		}
		std::cout << "\n";
	}
}