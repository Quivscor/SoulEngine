#pragma once
#include "Core.h"
#include "System.h"
#include "Shader.h"
#include "Window.h"
#include "Transform.h"

class Renderer : public System
{
public:
	Renderer(Shader* defaultShader);
	~Renderer();
	void Init() const;
	void Update() const;
	void LateUpdate() const;
	void DrawSquare(Transform* transform);

private:
	//list of meshes?
	Shader* defaultShader;
};