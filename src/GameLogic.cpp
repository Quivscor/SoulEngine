#include "GameLogic.h"

GameLogic::GameLogic()
{

}

GameLogic::~GameLogic()
{

}

void GameLogic::Init() const
{

}

void GameLogic::Start() const
{
	for (int i = 0; i < m_Entities.size(); i++)
		m_Entities[i]->GetComponent<ScriptableObject>()->Start();
}

void GameLogic::Update() const
{
	for (int i = 0; i < m_Entities.size(); i++)
		m_Entities[i]->GetComponent<ScriptableObject>()->Update();
}

void GameLogic::LateUpdate() const
{

}