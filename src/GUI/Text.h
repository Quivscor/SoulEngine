#pragma once
#include "Core.h"
#include "Component.h"
#include "Transform.h"
#include "EntityManager.h"

class Text : public Component
{

public:
	Text();
	Text(std::string text, glm::vec3 color);
	~Text();

	virtual ComponentType GetComponentType() const override { return ComponentType::TextComponent; }

	std::string text = "";

	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

};