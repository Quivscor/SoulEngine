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

//#include "Scripts/Water.h"

//#include "Model.h"
//class Mesh;
class Collider;
//class Model;
class Renderer : public System
{
public:
	Renderer(Shader* shader);
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

	//Debug
	void SetCamera(std::shared_ptr<Entity> camera);
	bool debugMode = false;
	//void DebugSetProjectionView(Transform* projection, Camera* view);
	//std::vector<Mesh> meshes;
	
private:
	//list of meshes?
	void DrawMeshes() const;
	void DrawGrass() const;
	
	void DrawFrustum(Frustum frustum) const;
	void DrawColliders(std::shared_ptr<Collider> col, std::shared_ptr<Transform> trns) const;
	static Shader* defaultShader;

	//Debug
	std::shared_ptr<Entity> mainCamera;
	//glm::mat4 camProjection;
	//glm::mat4 camView;
};