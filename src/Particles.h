#include <glad/glad.h>
#include<Core.h>
#include <map>
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "stb_image.h"
// CPU representation of a particle
const int MaxParticles = 100000;
struct SingleParticle {
	glm::vec3 pos, speed;
	unsigned char r, g, b, a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const SingleParticle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

class Particles : public Component
{
public:
	bool Active=false;
	SingleParticle ParticlesContainer[MaxParticles];
	int LastUsedParticle = 0;
	Particles();
	void Init();
	~Particles();
	void SortParticles();
	int FindUnusedParticle();
	//void setLife(float Life);
	//Particles(char* imagepath, bool x);
	virtual ComponentType GetComponentType() const override { return ComponentType::ParComponent; }
	GLuint loadDDS(const char* imagepath);
	void Draw(std::shared_ptr<Entity>  camera, glm::vec3 position, glm::vec2 size);
	void SetParticles(char* imagepath, bool x);
	Shader* shaderpar;
	bool type = true;
	//GLuint loadDDS(const char* imagepath);
	GLuint particles_color_buffer;
	GLuint particles_position_buffer;
	//float LifeLevel = 100.0f;
	GLuint Texture;
	GLuint billboard_vertex_buffer;

private:
	// texture;
	GLuint vao;
};
