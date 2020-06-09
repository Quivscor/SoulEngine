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

//#include "Scripts/Water.h"

//#include "Model.h"
//class Mesh;
class Billboard;
class Collider;
//class Model;
class Renderer : public System
{
public:
	Renderer(Shader* shader, Shader* screenShader);
	~Renderer();
	void Init() const;
	virtual void Update() const;
	void DebugUpdate();
	virtual void LateUpdate() const;
	void DrawCube(std::shared_ptr<Transform> transform, std::shared_ptr<Material> material);
	
	static Shader* GetDefualtShader();
	void RegisterManager(std::shared_ptr<InstanceManager> instanceManager);
	std::vector <std::shared_ptr <InstanceManager>> instanceManagers;
	std::vector <int> test;

	SystemType GetSystemType() const override { return SystemType::Renderer; }
	Billboard *box, *box2;
	//Shader* shader;
	glm::vec3 lightPos;
	Shader* simpleDepthShader;
	Shader* debugDepthQuad;
	//Debug
	void SetCamera(std::shared_ptr<Entity> camera);
	bool debugMode = false;
	//void DebugSetProjectionView(Transform* projection, Camera* view);
	//std::vector<Mesh> meshes;
	
private:
	//list of meshes?
	void DrawMeshes() const;
	void DrawGrass() const;
	void DrawGUI() const;
	void DrawFrustum(Frustum frustum) const;
	void DrawColliders(std::shared_ptr<Collider> col, std::shared_ptr<Transform> trns) const;
	static Shader* defaultShader;
	static Shader* screenShader;
	const static unsigned int SHADOW_WIDTH = 1024 ;
	const static unsigned int SHADOW_HEIGHT = 1024;
	unsigned   int depthMapFBO = 0;
	
	GLuint depthMap=0;
	const  float near_plane = 1.0f;
	const  float far_plane = 100.5f;

	unsigned int frameBuffer;
	unsigned int textureColorBuffer;
	unsigned int rbo;
	unsigned int quadVAO, quadVBO;

	//Debug
	std::shared_ptr<Entity> mainCamera;
	//glm::mat4 camProjection;
	//glm::mat4 camView;
};