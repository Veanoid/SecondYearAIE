#pragma once
#include "glm/glm.hpp"
class Mesh
{
public:
	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {}
	virtual ~Mesh();

	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 textCoord;
	};

	void initialiseQuad();
	void initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount = 0, unsigned int* indices = nullptr);
	virtual void draw();


protected:

	unsigned triCount;
	unsigned vao, vbo, ibo;
};

