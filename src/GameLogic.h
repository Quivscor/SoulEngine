#pragma once
#include "Core.h"
#include "System.h"
#include "Shader.h"
#include "ScriptableObject.h"
#include "System.h"

class GameLogic : public System
{
public:
	GameLogic();
	~GameLogic();

	void Init() const;
	void Start() const;
	virtual void Update() const;
	virtual void LateUpdate() const;

	SystemType GetSystemType() const override { return SystemType::GameLogic; }

private:
};