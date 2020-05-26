#include "ScriptableObject.h"
#include "Mesh.h"
#include "AssetManager.h"
#include "Shader.h"

class Water : public Component
{
public:
	Water();
	~Water();

	float waveSpeed = 1.0f;

	void CreateShape();

	virtual ComponentType GetComponentType() const { return ComponentType::WaterComponent; }

private:
	float width = 75.0f;
	float height = 75.0f;
	int xRectangles = 75;
	int zRectangles = 75;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec4> indicators;
};