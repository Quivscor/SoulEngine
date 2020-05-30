#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	//this->textures = textures;

	material = new Material();
	material->SetShader(Renderer::GetDefualtShader());
	material->SetTextures(textures);

	setupMesh();
}

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// normal vectors of vertices
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	// coordinates of the texture vertices
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	
	glBindVertexArray(0);
}

void Mesh::setupMesh(std::vector<glm::vec4> indicators)
{
	hasEBO = false;

	std::vector<VertexWithIndicators> verticesWithIndicatos;

	for (int i = 0; i < vertices.size(); i++)
	{
		VertexWithIndicators vwi;
		vwi.Position = vertices[i].Position;
		vwi.Normal = vertices[i].Normal;
		vwi.TexCoords = vertices[i].TexCoords;
		vwi.Indicators = indicators[i];

		verticesWithIndicatos.push_back(vwi);
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, verticesWithIndicatos.size() * sizeof(VertexWithIndicators), &verticesWithIndicatos[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
	//	&indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithIndicators), (void*)0);

	// normal vectors of vertices
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithIndicators), (void*)offsetof(VertexWithIndicators, Normal));

	// coordinates of the texture vertices
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexWithIndicators), (void*)offsetof(VertexWithIndicators, TexCoords));

	// normal vectors of vertices
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(VertexWithIndicators), (void*)offsetof(VertexWithIndicators, Indicators));

	glBindVertexArray(0);
}

void Mesh::SetAll(Mesh m)
{
	vertices = m.vertices;
	indices = m.indices;
	material = m.material;
	bones_id_weights_for_each_vertex = m.bones_id_weights_for_each_vertex;
}

unsigned int Mesh::GetVAO() const
{
	return VAO;
}

unsigned int Mesh::GetVBO()
{
	return VBO;
}

unsigned int Mesh::GetEBO()
{
	return EBO;
}
void Mesh::Draw(Shader* shaders_program)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	for (int i = 0; i < material->GetTextures().size(); i++)
	{
		std::string number;
		std::string name = material->GetTextures()[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		//defaultShader->setFloat(("material." + name + number).c_str(), i);
		glUniform1i(glGetUniformLocation(shaders_program->ID, (name + number).c_str()),i);
		glBindTexture(GL_TEXTURE_2D, material->GetTextures()[i].id);
	}

	//glUniform1f(glGetUniformLocation(shaders_program, "material.shininess"), 32.0f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glLineWidth(2);
	//Draw
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (int i = 0; i < material->GetTextures().size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
Mesh::Mesh(std::vector<Vertex> vertic, std::vector<GLuint> ind, std::vector<Texture> textures, std::vector<VertexBoneData> bone_id_weights)
{
	vertices = vertic;
	indices = ind;
	material = new Material();
	material->SetShader(Renderer::GetDefualtShader());
	material->SetTextures(textures);
	bones_id_weights_for_each_vertex = bone_id_weights;

	// Now that we have all the required data, set the vertex buffers and its attribute pointers.
	setupMeshfBones();
}
void VertexBoneData::addBoneData(uint bone_id, float weight)
{
	for (uint i = 0; i < NUM_BONES_PER_VEREX; i++)
	{
		if (weights[i] == 0.0)
		{
			ids[i] = bone_id;
			weights[i] = weight;
			return;
		}
	}
}

void Mesh::setupMeshfBones()
{
	//vertices data
	glGenBuffers(1, &VBO_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//bones data
	glGenBuffers(1, &VBO_bones);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bones);
	glBufferData(GL_ARRAY_BUFFER, bones_id_weights_for_each_vertex.size() * sizeof(bones_id_weights_for_each_vertex[0]), &bones_id_weights_for_each_vertex[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//numbers for sequence indices
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// create VAO and binding data from buffers to shaders
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1); // offsetof(Vertex, normal) = returns the byte offset of that variable from the start of the struct
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//bones
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bones);
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (GLvoid*)0); // for INT Ipointer
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (GLvoid*)offsetof(VertexBoneData, weights));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindVertexArray(0);
}

//void Mesh::SetDrawRadius(float radius)
//{
//	drawRadius = radius;
//}
//
//float Mesh::GetDrawRadius()
//{
//	return this->drawRadius;
//}
