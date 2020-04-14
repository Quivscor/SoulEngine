#pragma once
#include "Core.h"
#include "System.h"
#include "Shader.h"
#include "Window.h"
#include "Transform.h"
#include "Mesh.h"

class Mesh;

class Renderer : public System
{
public:
	Renderer(Shader* shader);
	~Renderer();
	void Init() const;
	virtual void Update() const;
	virtual void LateUpdate() const;
	void DrawCube(Transform* transform, Material* material);
	
	static Shader* GetDefualtShader();

	SystemType GetSystemType() const override { return SystemType::Renderer; }


	//Debug
	void DebugSetProjectionView(Transform* projection, Camera* view);
	std::vector<Mesh> meshes;

private:
	//list of meshes?
	void DrawMeshes() const;
	static Shader* defaultShader;

	//Debug
	Camera* camProjection;
	Transform* camView;
};