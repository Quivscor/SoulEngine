#pragma once
#include "Core.h"
#include "System.h"
#include "Shader.h"
#include "Window.h"
#include "Transform.h"
#include "Mesh.h"
#include "Collider.h"
#include "Frustum.h"
#include "InstanceManager.h"
#include "GUI/Text.h"
//#include <HUD.h>

//#include "Scripts/Water.h"

//#include "Model.h"
//class Mesh;
class Billboard;
class Collider;
//class Model;
class Renderer : public System
{
public:
	Renderer(Shader* shader, Shader* screenShader, Shader* skyBoxShader, Shader*refractorShader, Model * crystal);
	~Renderer();
	void Init();
	virtual void Update() const;
	void DebugUpdate();
	virtual void LateUpdate() const;
	void DrawCube(std::shared_ptr<Transform> transform, std::shared_ptr<Material> material);
	void RegisterBillboard(std::shared_ptr<Entity> billboard);
	
	static Shader* GetDefualtShader();
	void RegisterManager(std::shared_ptr<InstanceManager> instanceManager);
	std::vector <std::shared_ptr <InstanceManager>> instanceManagers;
	std::vector <int> test;

	SystemType GetSystemType() const override { return SystemType::Renderer; }
	//Billboard *box, *box2;
	//Shader* shader;
	glm::vec3 lightPos;
	Shader* simpleDepthShader;

	Shader* skyBoxShader;

	Shader* refractorShader;
	//Debug
	void SetCamera(std::shared_ptr<Entity> camera);
	bool debugMode = false;
	//void DebugSetProjectionView(Transform* projection, Camera* view);
	//std::vector<Mesh> meshes;

	bool berserkerModeActive = false;
	
private:
	//list of meshes?
	void DrawMeshes() const;
	void DrawGrass() const;
	void DrawGUI() const;
	void DrawHPbar() const;
	void DrawShadows() const;
	void DrawColliders(std::shared_ptr<Collider> col, std::shared_ptr<Transform> trns) const;
	static Shader* defaultShader;
	static Shader* screenShader;
	const static unsigned int SHADOW_WIDTH = 1024 ;
	const static unsigned int SHADOW_HEIGHT = 1024;
	unsigned   int depthMapFBO = 0;
	
	GLuint depthMap=0;
	const  float near_plane = 1.0f;
	const  float far_plane = 100.5f;
//	HUD* hud;
	unsigned int frameBuffer;
	unsigned int textureColorBuffer;
	unsigned int rbo;
	unsigned int quadVAO, quadVBO;
	unsigned int skyboxVAO, skyboxVBO;
	unsigned int cubemapTexture;
	Model* crystal;
	//Debug
	std::shared_ptr<Entity> mainCamera;
	std::shared_ptr<Camera> cameraComponent;
	std::shared_ptr<Transform> cameraTransform;
	std::vector<std::pair<std::shared_ptr<Billboard>, std::shared_ptr<Transform>>> billboards;
	//glm::mat4 camProjection;
	//glm::mat4 camView;

	void SortEntities();
};

unsigned int loadCubemap(std::vector<std::string> faces);
