#pragma once
#include "Core.h"
#include "System.h"
#include "Shader.h"
#include "Window.h"
#include "Transform.h"
#include "Material.h"
#include "Mesh.h"

class Renderer : public System
{
public:
	Renderer(Shader* defaultShader);
	~Renderer();
	void Init() const;
	virtual void Update() const;
	virtual void LateUpdate() const;
	void DrawCube(Transform* transform, Material* material);

	//Debug
	void DebugSetProjectionView(Transform* projection, Camera* view);
	std::vector<Mesh> meshes;

private:
	//list of meshes?
	void DrawMeshes() const;
	Shader* defaultShader;

	//Debug
	Camera* camProjection;
	Transform* camView;
};