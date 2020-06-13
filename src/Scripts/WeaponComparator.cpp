#include "WeaponComparator.h"

WeaponComparator::WeaponComparator()
{

}

WeaponComparator::~WeaponComparator()
{

}

void WeaponComparator::Show(bool value)
{
	EntityManager::GetInstance()->GetEntity(damageTitle->GetEntityID())->isActive = value;
	EntityManager::GetInstance()->GetEntity(oldDamage->GetEntityID())->isActive = value;
	EntityManager::GetInstance()->GetEntity(damageCompare->GetEntityID())->isActive = value;
	EntityManager::GetInstance()->GetEntity(newDamage->GetEntityID())->isActive = value;

	EntityManager::GetInstance()->GetEntity(speedTitle->GetEntityID())->isActive = value;
	EntityManager::GetInstance()->GetEntity(oldASpeed->GetEntityID())->isActive = value;
	EntityManager::GetInstance()->GetEntity(speedCompare->GetEntityID())->isActive = value;
	EntityManager::GetInstance()->GetEntity(newASpeed->GetEntityID())->isActive = value;

	EntityManager::GetInstance()->GetEntity(durabilityTitle->GetEntityID())->isActive = value;
	EntityManager::GetInstance()->GetEntity(oldDurability->GetEntityID())->isActive = value;
	EntityManager::GetInstance()->GetEntity(durabilityCompare->GetEntityID())->isActive = value;
	EntityManager::GetInstance()->GetEntity(newDurability->GetEntityID())->isActive = value;
}

void WeaponComparator::UpdateStats(float base, float oldValue, float newValue, std::shared_ptr<Entity> oldText, std::shared_ptr<Entity> newText)
{
	std::shared_ptr<Text> oldTextGUI = oldText->GetComponent<Text>();
	std::shared_ptr<Text> newTextGUI = newText->GetComponent<Text>();
	
	int oldCalcValue;
	int newCalcValue;

	if (base > 0)
	{
		oldCalcValue = (oldValue / base * 100);
		newCalcValue = (newValue / base * 100);
	}
	else
	{
		oldCalcValue = oldValue;
		newCalcValue = newValue;
	}


	oldTextGUI->text = std::to_string(oldCalcValue);
	newTextGUI->text = std::to_string(newCalcValue);

	if (oldValue > newValue)
	{
		oldTextGUI->color = glm::vec3(0.0f, 1.0f, 0.0f);
		newTextGUI->color = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else if (oldValue < newValue)
	{
		oldTextGUI->color = glm::vec3(1.0f, 0.0f, 0.0f);
		newTextGUI->color = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else
	{
		oldTextGUI->color = glm::vec3(0.8f, 0.8f, 0.8f);
		newTextGUI->color = glm::vec3(0.8f, 0.8f, 0.8f);
	}
}