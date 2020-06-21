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
	float pos[3] = { 0 };
	float scale[3] = { 0 };
	std::string name = "";
	Shader* shaderForEnemy = new Shader("./res/shaders/anim.vert", "./res/shaders/anim.frag");

	PrepareColliders();
	LoadMapModels();

	Model* grassLeaf = assetManager->LoadModel("./res/models/tiles/Grass/GrassFromGalapagos.obj");
	Model* auraRing = assetManager->LoadModel("./res/models/auraring.obj");
	Model* auraRingRed = assetManager->LoadModel("./res/models/Auras/AuraRingRed.obj");
	Model* auraRingBlue = assetManager->LoadModel("./res/models/Auras/AuraRingBlue.obj");
	Model* auraRingYellow = assetManager->LoadModel("./res/models/Auras/AuraRingYellow.obj");
	InstanceManager* grassM = new InstanceManager(grassLeaf);
	std::shared_ptr <InstanceManager> grassManager(grassM);
	
	srand(time(NULL));

	mapSizeX = GenerateRandomNumber(7, 12);
	mapSizeY = GenerateRandomNumber(7, 12);

	mapSizeX = 6;
	mapSizeY = 6;
	std::cout << "\n=== Map size";
	std::cout << "X: " << mapSizeX << " Y: " << mapSizeY << std::endl;

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
	int grassNumberOnTile = 150;
	grassManager->instanceModels = new glm::mat4[tilesNumber * grassNumberOnTile];
	grassManager->amount = tilesNumber * grassNumberOnTile;
	int numberOfGrass = grassManager->amount;
	int grassCounter = 0;
	std::cout << "\n=== Spawning tiles\n";

	
	for (int i = -1; i <= mapSizeX; i++)
	{
		for (int j = -1; j <= mapSizeY; j++)
		{
			if (i == -1 || j == -1 || i == mapSizeX || j == mapSizeY)
			{
				std::shared_ptr<Entity> tile = m_EntityManager->CreateEntity<Entity>();
				tile->AddComponent<Transform>();
				map.push_back(tile);
				renderer->RegisterEntity(tile);
				physics->RegisterEntity(tile);

				tile->AddComponent<Collider>();
				tile->GetComponent<Collider>()->SetShape(waterCollider);
				tile->GetComponent<Collider>()->isStatic = true;

				tile->GetComponent<Transform>()->SetLocalPosition(glm::vec3(j * 16, 0, i * 16));
			}
			
		}
	}
	// spawning tiles and grass
	std::ifstream file;
	int auraType = 0;
	for (int i = 0; i < mapSizeX; i++)
	{
		for (int j = 0; j < mapSizeY; j++)
		{
			
			if (generatedMap[i][j] == "Forest1")
			{
				file.open("./res/maps/TileForest.txt");
				if (!file)
				{
					std::cout << "Unable to open TileForest.txt";
				}
			}
			if (generatedMap[i][j] == "Forest2")
			{
				file.open("./res/maps/TileForest2.txt");
				if (!file)
				{
					std::cout << "Unable to open file Forest2.txt";
				}
			}
			if (generatedMap[i][j] == "Village")
			{
				file.open("./res/maps/TileVillage.txt");
				if (!file)
				{
					std::cout << "Unable to open file TileVillage.txt";
				}
			}
			if (generatedMap[i][j] == "Mountains")
			{
				file.open("./res/maps/TileMountain.txt");
				if (!file)
				{
					std::cout << "Unable to open file TileMountain.txt";
				}
			}
			if (generatedMap[i][j] == "GrassLand")
			{
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

			if (generatedMap[i][j] == "Water")
			{
				tile->AddComponent<Collider>();
				tile->GetComponent<Collider>()->SetShape(waterCollider);
				tile->GetComponent<Collider>()->isStatic = true;
			}

			if (generatedMap[i][j] == "Village")
			{
				tile->AddComponent<Village>();
				gameLogic->RegisterEntity(tile);

				auraType = rand() % 3; 

				if (auraType == 0)
					tile->GetComponent<Village>()->auras.push_back(new AuraBonusDamage());
				else if (auraType == 1)
					tile->GetComponent<Village>()->auras.push_back(new AuraBonusHealth());
				else if (auraType == 2)
					tile->GetComponent<Village>()->auras.push_back(new AuraRunningSpeed());

				int bonusAuraChance = rand() % 10;

				if (bonusAuraChance <= 1)
				{
					int oldAuraType = auraType;

					while (oldAuraType == auraType)
						auraType = rand() % 3;

					if (auraType == 0)
						tile->GetComponent<Village>()->auras.push_back(new AuraBonusDamage());
					else if (auraType == 1)
						tile->GetComponent<Village>()->auras.push_back(new AuraBonusHealth());
					else if (auraType == 2)
						tile->GetComponent<Village>()->auras.push_back(new AuraRunningSpeed());
				}
			}

			while (file >> name >> pos[0] >> pos[1] >> pos[2] >> scale[0] >> scale[1] >> scale[2])
			{
				std::shared_ptr<Entity> object = m_EntityManager->CreateEntity<Entity>();

				if (name == "Weapon")
				{
					if (rand() % 100 >= 50)
					{
						object->AddComponent<Transform>();
						object->GetComponent<Transform>()->SetParent(tile->GetComponent<Transform>());
						object->GetComponent<Transform>()->SetLocalPosition(glm::vec3(pos[0], pos[1]+0.35f, pos[2]));
						object->GetComponent<Transform>()->SetLocalScale(glm::vec3(scale[0]*2, scale[1]*2, scale[2]*2));

						object->AddComponent<Material>();
						object->AddComponent<WeaponOnTheGround>();
						object->GetComponent<WeaponOnTheGround>()->Start();
						physics->RegisterEntity(object);
						renderer->RegisterEntity(object);
					}
					
				}
				else
				{
					object->AddComponent<Transform>();
					object->GetComponent<Transform>()->SetParent(tile->GetComponent<Transform>());
					object->GetComponent<Transform>()->SetLocalPosition(glm::vec3(pos[0], pos[1], pos[2]));
					object->GetComponent<Transform>()->SetLocalScale(glm::vec3(scale[0], scale[1], scale[2]));

					object->AddComponent<Mesh>();
					object->GetComponent<Mesh>()->indices = (FindModelByName(tileModels, name))->GetMeshes()[0].indices;
					object->GetComponent<Mesh>()->vertices = (FindModelByName(tileModels, name))->GetMeshes()[0].vertices;
					object->GetComponent<Mesh>()->material = (FindModelByName(tileModels, name))->GetMeshes()[0].material;
					object->GetComponent<Mesh>()->setupMesh();
				}

				if (name == "Grass")
				{
					object->layer = Layer::GroundLayer;
					
				}
				if (name == "Tree" || name == "Birch" )
				{
					float treeX = rand() % 100 / 1000.0f;
					float treeY = rand() % 100 / 1000.0f;
					float treeZ = rand() % 100 / 1000.0f;
					if (rand() % 100 <= 10)
					{
						treeX *= -1.f;
						treeY *= -1.f;
						treeZ *= -1.f;
					}

					object->GetComponent<Transform>()->SetLocalScale(glm::vec3(scale[0] + treeX, scale[1] + treeY, scale[2] + treeZ));
					object->AddComponent<Collider>();
					object->GetComponent<Collider>()->SetShape(treeCollider);
					object->GetComponent<Collider>()->isStatic = true;
				}
				if (name == "Rock1" || name == "Rock2" || name == "Rock3")
				{
					
					if (generatedMap[i][j] == "Mountains")
					{
						std::shared_ptr<Entity> rockScale = m_EntityManager->CreateEntity<Entity>();
						rockScale->AddComponent<Transform>();
						rockScale->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.5, 0.5, 0.5));
						rockScale->GetComponent<Transform>()->SetParent(object->GetComponent<Transform>());
						rockScale->AddComponent<Collider>();
						rockScale->GetComponent<Collider>()->SetShape(rockCollider);
						rockScale->GetComponent<Collider>()->isStatic = true;
						physics->RegisterEntity(rockScale);
					}	
					else
					{
						object->AddComponent<Collider>();
						object->GetComponent<Collider>()->SetShape(rockCollider);
						object->GetComponent<Collider>()->isStatic = true;
						if (generatedMap[i][j] == "Village")
						{
							if (tile->GetComponent<Village>()->auras.size() == 1)
							{
								std::shared_ptr<Entity> aura = m_EntityManager->CreateEntity<Entity>();
								aura->AddComponent<Transform>();
								aura->GetComponent<Transform>()->SetParent(object->GetComponent<Transform>());
								aura->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0, 0.1f, 0));
								aura->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.7f, 0.3f, 0.7f));
								
								if (tile->GetComponent<Village>()->auras[0]->ToString() == "AuraBonusDamage")
								{
									aura->AddComponent<Mesh>();
									aura->GetComponent<Mesh>()->indices = auraRingRed->GetMeshes()[0].indices;
									aura->GetComponent<Mesh>()->vertices = auraRingRed->GetMeshes()[0].vertices;
									aura->GetComponent<Mesh>()->material = auraRingRed->GetMeshes()[0].material;
									aura->GetComponent<Mesh>()->setupMesh();
								}
									
								if (tile->GetComponent<Village>()->auras[0]->ToString() == "AuraBonusHealth")
								{
									aura->AddComponent<Mesh>();
									aura->GetComponent<Mesh>()->indices = auraRingBlue->GetMeshes()[0].indices;
									aura->GetComponent<Mesh>()->vertices = auraRingBlue->GetMeshes()[0].vertices;
									aura->GetComponent<Mesh>()->material = auraRingBlue->GetMeshes()[0].material;
									aura->GetComponent<Mesh>()->setupMesh();
								}
									
								if (tile->GetComponent<Village>()->auras[0]->ToString() == "AuraRunningSpeed")
								{
									aura->AddComponent<Mesh>();
									aura->GetComponent<Mesh>()->indices = auraRingYellow->GetMeshes()[0].indices;
									aura->GetComponent<Mesh>()->vertices = auraRingYellow->GetMeshes()[0].vertices;
									aura->GetComponent<Mesh>()->material = auraRingYellow->GetMeshes()[0].material;
									aura->GetComponent<Mesh>()->setupMesh();
								}
									
								renderer->RegisterEntity(aura);
								physics->RegisterEntity(aura);
							}
							if (tile->GetComponent<Village>()->auras.size() == 2)
							{
								
								std::shared_ptr<Entity> aura = m_EntityManager->CreateEntity<Entity>();
								aura->AddComponent<Transform>();
								aura->GetComponent<Transform>()->SetParent(object->GetComponent<Transform>());
								aura->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0, 0.0f, 0));
								aura->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.7f, 0.3f, 0.7f));

								if (tile->GetComponent<Village>()->auras[0]->ToString() == "AuraBonusDamage")
								{
									aura->AddComponent<Mesh>();
									aura->GetComponent<Mesh>()->indices = auraRingRed->GetMeshes()[0].indices;
									aura->GetComponent<Mesh>()->vertices = auraRingRed->GetMeshes()[0].vertices;
									aura->GetComponent<Mesh>()->material = auraRingRed->GetMeshes()[0].material;
									aura->GetComponent<Mesh>()->setupMesh();
								}

								if (tile->GetComponent<Village>()->auras[0]->ToString() == "AuraBonusHealth")
								{
									aura->AddComponent<Mesh>();
									aura->GetComponent<Mesh>()->indices = auraRingBlue->GetMeshes()[0].indices;
									aura->GetComponent<Mesh>()->vertices = auraRingBlue->GetMeshes()[0].vertices;
									aura->GetComponent<Mesh>()->material = auraRingBlue->GetMeshes()[0].material;
									aura->GetComponent<Mesh>()->setupMesh();
								}

								if (tile->GetComponent<Village>()->auras[0]->ToString() == "AuraRunningSpeed")
								{
									aura->AddComponent<Mesh>();
									aura->GetComponent<Mesh>()->indices = auraRingYellow->GetMeshes()[0].indices;
									aura->GetComponent<Mesh>()->vertices = auraRingYellow->GetMeshes()[0].vertices;
									aura->GetComponent<Mesh>()->material = auraRingYellow->GetMeshes()[0].material;
									aura->GetComponent<Mesh>()->setupMesh();
								}
						
								renderer->RegisterEntity(aura);
								physics->RegisterEntity(aura); 


								std::shared_ptr<Entity> aura2 = m_EntityManager->CreateEntity<Entity>();
								aura2->AddComponent<Transform>();
								aura2->GetComponent<Transform>()->SetParent(object->GetComponent<Transform>());
								aura2->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0, 0.3f, 0));
								aura2->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.7f, 0.3f, 0.7f));

								if (tile->GetComponent<Village>()->auras[1]->ToString() == "AuraBonusDamage")
								{
									aura2->AddComponent<Mesh>();
									aura2->GetComponent<Mesh>()->indices = auraRingRed->GetMeshes()[0].indices;
									aura2->GetComponent<Mesh>()->vertices = auraRingRed->GetMeshes()[0].vertices;
									aura2->GetComponent<Mesh>()->material = auraRingRed->GetMeshes()[0].material;
									aura2->GetComponent<Mesh>()->setupMesh();
								}

								if (tile->GetComponent<Village>()->auras[1]->ToString() == "AuraBonusHealth")
								{
									aura2->AddComponent<Mesh>();
									aura2->GetComponent<Mesh>()->indices = auraRingBlue->GetMeshes()[0].indices;
									aura2->GetComponent<Mesh>()->vertices = auraRingBlue->GetMeshes()[0].vertices;
									aura2->GetComponent<Mesh>()->material = auraRingBlue->GetMeshes()[0].material;
									aura2->GetComponent<Mesh>()->setupMesh();
								}

								if (tile->GetComponent<Village>()->auras[1]->ToString() == "AuraRunningSpeed")
								{
									aura2->AddComponent<Mesh>();
									aura2->GetComponent<Mesh>()->indices = auraRingYellow->GetMeshes()[0].indices;
									aura2->GetComponent<Mesh>()->vertices = auraRingYellow->GetMeshes()[0].vertices;
									aura2->GetComponent<Mesh>()->material = auraRingYellow->GetMeshes()[0].material;
									aura2->GetComponent<Mesh>()->setupMesh();
								}
								renderer->RegisterEntity(aura2);
								physics->RegisterEntity(aura2);
							}
							
						}

					}
					
				}
				if (name == "House1")
				{
					object->GetComponent<Transform>()->SetLocalScale(glm::vec3(scale[0] + 0.1f, scale[1] + 0.1f, scale[2] + 0.1f));
					object->AddComponent<Collider>();
					object->GetComponent<Collider>()->SetShape(house1Collider);
					object->GetComponent<Collider>()->isStatic = true;
				}
				if (name == "House2")
				{
					object->GetComponent<Transform>()->SetLocalScale(glm::vec3(scale[0] + 0.1f, scale[1] + 0.1f, scale[2] + 0.1f));
					object->AddComponent<Collider>();
					object->GetComponent<Collider>()->SetShape(house2Collider);
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
					object->AddComponent<Billboard>();
					object->GetComponent<Billboard>()->SetBillboard("./res/textures/ExampleBillboard.DDS", true);
					object->GetComponent<Character>()->healthBar = object->GetComponent<Billboard>();

					renderer->RegisterBillboard(object);

					player->GetComponent<PlayerEnemyCommunicator>()->EnemySpawned();

					object->layer = EnemyLayer;

					//enemy weapon
					std::shared_ptr<Entity> weapon = m_EntityManager->CreateEntity<Entity>();
					weapon->AddComponent<Transform>();
					weapon->GetComponent<Transform>()->SetParent(object->GetComponent<Transform>());
					weapon->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));

					std::vector<glm::vec2> weaponColliderShape;
					weaponColliderShape.clear();
					weaponColliderShape.push_back({ -6.0f, -5.0f });
					weaponColliderShape.push_back({ -6.0f, 18.0f });
					weaponColliderShape.push_back({ 6.0f, 18.0f });
					weaponColliderShape.push_back({ 6.0f, -5.0f });

					weapon->AddComponent<Collider>();
					weapon->GetComponent<Collider>()->SetShape(weaponColliderShape);
					weapon->GetComponent<Collider>()->isTrigger = true;
					weapon->AddComponent<EnemyWeapon>();

					renderer->RegisterEntity(weapon);
					gameLogic->RegisterEntity(weapon);
					physics->RegisterEntity(weapon);

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

					enemy->GetComponent<Enemy>()->weapon = weapon;
					if (tile->GetComponent<Village>()->auras.size() == 1)
					{
						std::shared_ptr<Entity> aura = m_EntityManager->CreateEntity<Entity>();
						aura->AddComponent<Transform>();
						aura->GetComponent<Transform>()->SetParent(weapon->GetComponent<Transform>());
						aura->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0, 0.5f, 0));
						aura->GetComponent<Transform>()->SetLocalScale(glm::vec3(2.0f, 2.0f, 2.0f));

						if (tile->GetComponent<Village>()->auras[0]->ToString() == "AuraBonusDamage")
						{
							aura->AddComponent<Mesh>();
							aura->GetComponent<Mesh>()->indices = auraRingRed->GetMeshes()[0].indices;
							aura->GetComponent<Mesh>()->vertices = auraRingRed->GetMeshes()[0].vertices;
							aura->GetComponent<Mesh>()->material = auraRingRed->GetMeshes()[0].material;
							aura->GetComponent<Mesh>()->setupMesh();
						}

						if (tile->GetComponent<Village>()->auras[0]->ToString() == "AuraBonusHealth")
						{
							aura->AddComponent<Mesh>();
							aura->GetComponent<Mesh>()->indices = auraRingBlue->GetMeshes()[0].indices;
							aura->GetComponent<Mesh>()->vertices = auraRingBlue->GetMeshes()[0].vertices;
							aura->GetComponent<Mesh>()->material = auraRingBlue->GetMeshes()[0].material;
							aura->GetComponent<Mesh>()->setupMesh();
						}

						if (tile->GetComponent<Village>()->auras[0]->ToString() == "AuraRunningSpeed")
						{
							aura->AddComponent<Mesh>();
							aura->GetComponent<Mesh>()->indices = auraRingYellow->GetMeshes()[0].indices;
							aura->GetComponent<Mesh>()->vertices = auraRingYellow->GetMeshes()[0].vertices;
							aura->GetComponent<Mesh>()->material = auraRingYellow->GetMeshes()[0].material;
							aura->GetComponent<Mesh>()->setupMesh();
						}

						renderer->RegisterEntity(aura);
						physics->RegisterEntity(aura);
					}
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
			if (generatedMap[i][j]!= "Water")
			{
				name = "GrassLeaf";
				for (int k = 0; k < grassNumberOnTile; k++)
				{
					float randomX = 0;
					float randomY = 0;
					// circle in centre of villages with lesser grass
					if (generatedMap[i][j] == "Village")
					{
						float radius = 5.0f;
		
						randomX = rand() % 8 + (rand() % 100) / 100.f;
						if (rand() % 2 == 0)
							randomX *= -1;
						randomY = rand() % 8 + (rand() % 100) / 100.f;
						if (rand() % 2 == 0)
							randomY *= -1;

						if (randomX*randomX + randomY * randomY < radius*radius)
						{
							if (rand() % 100 > 5)
							{
								randomX = rand() % 8 + (rand() % 100) / 100.f;
								if (rand() % 2 == 0)
									randomX *= -1;
								randomY = rand() % 8 + (rand() % 100) / 100.f;
								if (rand() % 2 == 0)
									randomY *= -1;
							}
						}
						
					}
					else 
					{
						randomX = rand() % 8 + (rand() % 100) / 100.f;
						if (rand() % 2 == 0)
							randomX *= -1;
						randomY = rand() % 8 + (rand() % 100) / 100.f;
						if (rand() % 2 == 0)
							randomY *= -1;
					}
					
						
					
					float grassX = rand() % 150 / 1000.f;
					//if (rand() % 2 == 0)
						//grassX *=-1.f;
					float grassY = rand() % 150 / 1000.f;
					//if (rand() % 2 == 0)
						//grassY *=-1.f;
					float grassZ = rand() % 150 / 1000.f;
					//if (rand() % 2 == 0)
						//grassZ *=-1.f;
					if (rand() % 2 == 0)
					{
						grassX *= -1.0f;
						grassY *= -1.0f;
						grassZ *= -1.0f;
					}
					glm::vec3 pos(randomX + j * 16, grassY, randomY + i * 16);
					glm::vec3 scale(0.4 + grassX, 0.4 + grassY, 0.4 + grassZ);
					glm::vec3 rot(0, 0, 0);
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(pos));
					model = glm::scale(model, glm::vec3(scale));
					
					grassManager->instanceModels[grassCounter] = model;
					grassCounter++;
				

				}
			}
			tile->GetComponent<Transform>()->SetLocalPosition(glm::vec3(j * 16, 0, i * 16));
			std::cout << "- tile [" << i << "][" << j << "] " << generatedMap[i][j] << " spawned\n";
		}
	}
	std::shared_ptr<Entity> object = m_EntityManager->CreateEntity<Entity>();
	object->AddComponent<Mesh>();
	object->GetComponent<Mesh>()->indices = (grassLeaf)->GetMeshes()[0].indices;
	object->GetComponent<Mesh>()->vertices = (grassLeaf)->GetMeshes()[0].vertices;
	object->GetComponent<Mesh>()->material = (grassLeaf)->GetMeshes()[0].material;
	object->GetComponent<Mesh>()->setupMesh();
	grassManager->m_mesh = object->GetComponent<Mesh>().get();
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
	else return 0;
}
void MapGenerator::LoadMapModels()
{
	std::cout << "\n=== Loading map models\n";
	tileModels[0] = assetManager->LoadModel("./res/models/tiles/Grass/Grass.obj");
	std::cout << "- Grass.obj loaded \n";
	tileModels[1] = assetManager->LoadModel("./res/models/tiles/Tree/Tree.obj");
	std::cout << "- Tree.obj loaded\n";
	tileModels[2] = assetManager->LoadModel("./res/models/tiles/Tree/Birch.obj");
	std::cout << "- Birch.obj loaded \n";
	tileModels[3] = assetManager->LoadModel("./res/models/tiles/Rocks/Rock1.obj");
	std::cout << "- Rock1.obj loaded \n";
	tileModels[4] = assetManager->LoadModel("./res/models/tiles/Rocks/Rock2.obj");
	std::cout << "- Rock2.obj loaded \n";
	tileModels[5] = assetManager->LoadModel("./res/models/tiles/Rocks/Rock3.obj");
	std::cout << "- Rock3.obj loaded \n";
	tileModels[6] = assetManager->LoadModel("./res/models/tiles/Houses/house_1.obj");
	std::cout << "- house_1.obj loaded \n";
	tileModels[7] = assetManager->LoadModel("./res/models/tiles/Houses/house_2.obj");
	std::cout << "- house_2.obj loaded \n";
	tileModels[8] = assetManager->LoadModel("./res/models/player/player_idle.dae");
	std::cout << "- player_idle.dae loaded \n";
	tileModels[9] = assetManager->LoadModel("./res/models/player/player_idle.dae");
	std::cout << "- player_idle.dae loaded \n";
	tileModels[10] = assetManager->LoadModel("./res/models/player/player_run.dae");
	std::cout << "- player_run.dae loaded \n";
	tileModels[11] = assetManager->LoadModel("./res/models/player/player_attack.dae");
	std::cout << "- player_attack.dae loaded \n";
	tileModels[12] = assetManager->LoadModel("./res/models/player/player_death.dae");
	std::cout << "- player_death.dae loaded \n";
}
void MapGenerator::PrepareColliders()
{
	waterCollider.push_back({-8.f, -8.f});
	waterCollider.push_back({-8.f, 8.f });
	waterCollider.push_back({8.f, 8.f});
	waterCollider.push_back({8.f, -8.f });

	treeCollider.push_back({ -2.0f, -1.25f });
	treeCollider.push_back({ -1.25f, -2.0f });
	treeCollider.push_back({ 1.25f, -2.0f });
	treeCollider.push_back({ 2.0f, -1.25f });
	treeCollider.push_back({ 2.0f, 1.25f });
	treeCollider.push_back({ 1.25f,  2.0f });
	treeCollider.push_back({ -1.25f, 2.0f });
	treeCollider.push_back({ -2.0f, 1.25f });

	rockCollider.push_back({ -1.0f, -0.625f });
	rockCollider.push_back({ -0.625f, -1.0f });
	rockCollider.push_back({ 0.625f, -1.0f });
	rockCollider.push_back({ 1.0f, -0.625f });
	rockCollider.push_back({ 1.0f,  0.625f });
	rockCollider.push_back({ 0.625f,  1.0f });
	rockCollider.push_back({ -0.625f, 1.0f });
	rockCollider.push_back({ -1.0f,  0.625f });

	characterCollider.push_back({ -8.0f, -5.0f });
	characterCollider.push_back({ -5.0f, -8.0f });
	characterCollider.push_back({ 5.0f, -8.0f });
	characterCollider.push_back({ 8.0f, -5.0f });
	characterCollider.push_back({ 8.0f, 5.0f });
	characterCollider.push_back({ 5.0f,  8.0f });
	characterCollider.push_back({ -5.0f, 8.0f });
	characterCollider.push_back({ -8.0f, 5.0f });

	house1Collider.push_back({-4.0f, -6.0f});
	house1Collider.push_back({-4.0f, 10.0f});
	house1Collider.push_back({4.0f, 10.0f});
	house1Collider.push_back({4.0f, -6.0f});

	house2Collider.push_back({ -6.0f, -6.0f });
	house2Collider.push_back({ -6.0f, 10.0f });
	house2Collider.push_back({ 6.0f, 8.0f });
	house2Collider.push_back({ 6.0f, -6.0f });
}

int MapGenerator::GenerateRandomNumber(int min, int max)
{
	return  min + (rand() % static_cast<int>(max-1 - min + 1));
}
void MapGenerator::ShowMapShape()
{
	std::cout << "\n=== Map shape\n";
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
	CountTiles();

}
// counting number of lands/tiles on map after generation
void MapGenerator::CountTiles()
{
	for (int i = 0; i < mapSizeX; i++)
	{
		for (int j = 0; j < mapSizeY; j++)
		{
			if(mapShape[i][j])
				tilesNumber++;
		}
	}
}
void MapGenerator::CreateVillages()
{
	// starting tile is a grassland
	generatedMap[mapSizeX / 2][mapSizeY / 2] = "GrassLand";
	// one village is 2 tiles to north from player's spawn
	generatedMap[mapSizeX / 2 - 1][mapSizeY / 2] = "Village";

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
	std::cout << "\n=== Map of villages preset\n";
	//std::cout << "Legend\n. - water, O - Land, V - village, g - grassland\n";
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
	std::cout << "\n=== Map tiles\n";
	std::cout << "= Number of tiles: " << tilesNumber << "\n";
	//std::cout << "Legend\nV - village, F - forest1, f - forest2, g - grassland, m - mountains, . - water\n";
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