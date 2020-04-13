#pragma once
#include "Core.h"
#include "System.h"
#include "Shader.h"
#include "Window.h"
#include "Transform.h"
#include "Material.h"

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

private:
	//list of meshes?
	Shader* defaultShader;

	//Debug
	Camera* camProjection;
	Transform* camView;
};