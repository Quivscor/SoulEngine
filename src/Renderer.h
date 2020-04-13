#pragma once
#include "Core.h"
#include "System.h"
#include "Shader.h"
#include "Window.h"
#include "Transform.h"
#include "Material.h"

class Renderer : System
{
public:
	Renderer(int ID, Shader* defaultShader);
	~Renderer();
	virtual void Update() const;
	virtual void LateUpdate() const;
	void DrawCube(Transform* transform, Material* material);

private:
	//list of meshes?
	Shader* defaultShader;
};