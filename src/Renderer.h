#pragma once
#include "Core.h"
#include "System.h"
#include "Shader.h"
#include "Window.h"

class Renderer : public System
{
public:
	Renderer(int ID, Shader* defaultShader);
	~Renderer();
	void Init() const;
	void Update() const;
	void LateUpdate() const;
	void DrawSquare();

private:
	//list of meshes?
	Shader* defaultShader;
};