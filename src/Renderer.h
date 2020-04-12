#pragma once
#include "Core.h"
#include "System.h"
#include "Shader.h"
#include "Window.h"

class Renderer : System
{
public:
	Renderer(int ID, Shader* defaultShader);
	~Renderer();
	virtual void Update() const;
	virtual void LateUpdate() const;
	void DrawSquare();

private:
	//list of meshes?
	Shader* defaultShader;
};