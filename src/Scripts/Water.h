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
	float width = 15.0f;
	float height = 15.0f;
	int xRectangles = 15;
	int zRectangles = 15;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};